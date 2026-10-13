#include "McpCore.h"
#include <juce_events/juce_events.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
using namespace organic::mcp;
struct Request
{
    std::shared_ptr<juce::WebInputStream> stream;
    std::atomic<bool> finished {false};
    std::thread worker;
};
class Bridge
{
public:
    Bridge(String endpoint, String token) : endpoint(endpoint), token(token) {}
    ~Bridge()
    {
        stopping = true;
        std::vector<std::shared_ptr<Request>> pending;
        { std::lock_guard<std::mutex> lifetime(lifecycle); std::lock_guard<std::mutex> lock(mutex); for (auto& item : requests) pending.push_back(item.second); }
        for (auto& request : pending) { if (request->stream) request->stream->cancel(); }
        for (auto& request : pending) if (request->worker.joinable()) request->worker.join();
    }
    void run()
    {
        std::string line; bool overflow = false; char character;
        while (std::cin.get(character))
        {
            if (character != '\n')
            {
                if (line.size() < 1024 * 1024) line += character; else overflow = true;
                continue;
            }
            if (overflow) emit(Protocol::error({}, -32600, "Stdio message exceeds the payload limit"));
            else if (!line.empty())
                try { receive(parseBounded(String::fromUTF8(line.data(), (int) line.size()))); }
                catch (const Error& error) { emit(Protocol::error({}, error.code, error.what())); }
            line.clear(); overflow = false;
        }
    }
private:
    String endpoint, token, session, version;
    std::mutex mutex, outputMutex, lifecycle;
    std::map<String, std::shared_ptr<Request>> requests;
    std::atomic<bool> stopping {false};
    void emit(const var& message)
    {
        if (stopping) return;
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << juce::JSON::toString(message, true).toStdString() << '\n' << std::flush;
    }
    static String headerName(const String& value)
    {
        bool encode = value.trim() != value || (value.startsWith("=?base64?") && value.endsWith("?="));
        for (auto character : value) if (character < 0x20 || character > 0x7e) encode = true;
        return encode ? "=?base64?" + juce::Base64::toBase64(value.toRawUTF8(), value.getNumBytesAsUTF8()) + "?=" : value;
    }
    void receive(const var& message)
    {
        if (!message.getDynamicObject() || message["jsonrpc"].toString() != "2.0" || !message["method"].isString())
            throw Error(-32600, "Expected a JSON-RPC request or notification");
        auto method = message["method"].toString();
        if (method == "notifications/cancelled")
        {
            auto key = juce::JSON::toString(message["params"]["requestId"], true);
            std::lock_guard<std::mutex> lock(mutex);
            auto found = requests.find(key);
            if (found != requests.end() && found->second->stream) found->second->stream->cancel();
            // Modern HTTP cancellation is the close itself. Legacy servers may
            // also receive the notification through the normal forwarding path.
            if (version == "2026-07-28") return;
        }
        const bool initialized = method == "notifications/initialized";
        start(message, false, initialized);
        if (initialized) start(object(), true);
    }
    void start(var message, bool events, bool synchronous = false)
    {
        std::lock_guard<std::mutex> lifetime(lifecycle);
        if (stopping) return;
        auto id = message["id"]; auto method = message["method"].toString();
        String key = events ? String("legacy-events") : message.hasProperty("id") ? juce::JSON::toString(id, true) : juce::Uuid().toString();
        String selectedVersion, selectedSession;
        std::vector<std::shared_ptr<Request>> completed;
        auto request = std::make_shared<Request>();
        {
            std::lock_guard<std::mutex> lock(mutex);
            for (auto it = requests.begin(); it != requests.end();)
                if (it->second->finished) { completed.push_back(it->second); it = requests.erase(it); } else ++it;
            if (requests.count(key)) { emit(Protocol::error(id, -32600, "Request ID is already active")); return; }
            if (requests.size() >= 8) { emit(Protocol::error(id, -32000, "Bridge concurrency limit reached")); return; }
            selectedVersion = message["params"]["_meta"]["io.modelcontextprotocol/protocolVersion"].toString();
            if (selectedVersion.isEmpty()) selectedVersion = method == "initialize" ? message["params"]["protocolVersion"].toString() : version;
            selectedSession = session;
            String headers = "Authorization: Bearer " + token + "\r\nContent-Type: application/json\r\nAccept: application/json, text/event-stream\r\n";
            if (selectedVersion.isNotEmpty()) headers += "MCP-Protocol-Version: " + selectedVersion + "\r\n";
            if (selectedSession.isNotEmpty()) headers += "Mcp-Session-Id: " + selectedSession + "\r\n";
            if (selectedVersion == "2026-07-28")
            {
                headers += "Mcp-Method: " + method + "\r\n";
                if (method == "tools/call" || method == "prompts/get" || method == "resources/read")
                    headers += "Mcp-Name: " + headerName(message["params"][method == "resources/read" ? "uri" : "name"].toString()) + "\r\n";
            }
            auto url = events ? juce::URL(endpoint) : juce::URL(endpoint).withPOSTData(juce::JSON::toString(message, true));
            request->stream = std::make_shared<juce::WebInputStream>(url, !events);
            request->stream->withExtraHeaders(headers).withConnectionTimeout(5000).withNumRedirectsToFollow(0);
            requests[key] = request;
        }
        for (auto& previous : completed) if (previous->worker.joinable()) previous->worker.join();
        request->worker = std::thread([this, request, message, id, method, selectedVersion, events, synchronous]
        {
            bool openEvents = false;
            try
            {
                auto& stream = *request->stream;
                if (!stream.connect(nullptr)) throw Error(-32000, "Cannot connect to the selected Chataigne instance");
                auto status = stream.getStatusCode(); auto headers = stream.getResponseHeaders();
                if (headers["Content-Type"].contains("text/event-stream"))
                {
                    String data; int bytes = 0;
                    while (!stopping && !stream.isExhausted() && !stream.isError())
                    {
                        std::string line; char character;
                        while (stream.read(&character, 1) == 1 && character != '\n')
                        { if (++bytes > 1024 * 1024) throw Error(-32000, "SSE event exceeds the payload limit"); if (character != '\r') line += character; }
                        auto text = String::fromUTF8(line.data(), (int) line.size());
                        if (text.startsWith("data:")) data += text.substring(5).trimStart();
                        if (text.isEmpty() && data.isNotEmpty()) { emit(parseBounded(data)); data.clear(); bytes = 0; }
                        else if (text.startsWith(":")) bytes = 0;
                    }
                }
                else
                {
                    juce::MemoryOutputStream body;
                    char buffer[4096];
                    while (!stream.isExhausted() && !stream.isError())
                    {
                        int size = stream.read(buffer, sizeof(buffer)); if (size <= 0) break;
                        if (body.getDataSize() + (size_t) size > 1024 * 1024) throw Error(-32000, "HTTP response exceeds the payload limit");
                        body.write(buffer, (size_t) size);
                    }
                    if (method == "initialize" && status == 200)
                    {
                        auto response = parseBounded(body.toUTF8());
                        std::lock_guard<std::mutex> lock(mutex);
                        session = headers["Mcp-Session-Id"]; version = response["result"]["protocolVersion"].toString();
                    }
                    else if (selectedVersion == "2026-07-28") { std::lock_guard<std::mutex> lock(mutex); version = selectedVersion; }
                    if (message.hasProperty("id") && !events)
                    {
                        if (body.getDataSize() == 0) throw Error(-32000, "MCP endpoint returned an empty response (HTTP " + String(status) + ")");
                        emit(parseBounded(body.toUTF8()));
                    }
                    openEvents = method == "notifications/initialized" && status == 202;
                }
            }
            catch (const Error& error) { if (message.hasProperty("id") && !events) emit(Protocol::error(id, error.code, error.what())); }
            catch (const std::exception&) { if (message.hasProperty("id") && !events) emit(Protocol::error(id, -32603, "Native bridge transport error")); }
            if (openEvents && !stopping && !synchronous) start(object(), true);
            request->finished = true;
        });
        if (synchronous && request->worker.joinable()) request->worker.join();
    }
};
}
int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI runtime;
    juce::String endpoint, tokenEnvironment = "CHATAIGNE_MCP_TOKEN";
    for (int i = 1; i < argc; ++i)
    {
        juce::String argument(argv[i]);
        if (argument == "--endpoint" && i + 1 < argc) endpoint = argv[++i];
        else if (argument == "--token-env" && i + 1 < argc) tokenEnvironment = argv[++i];
        else { std::cerr << "Usage: chataigne-mcp-bridge --endpoint http://127.0.0.1:PORT/mcp [--token-env NAME]\n"; return 2; }
    }
    juce::URL url(endpoint);
    if (url.getScheme() != "http" || (url.getDomain() != "127.0.0.1" && url.getDomain() != "localhost")
        || url.getPort() < 1 || url.getSubPath() != "mcp" || endpoint.containsAnyOf("@?#\r\n"))
    { std::cerr << "Select an explicit authenticated loopback /mcp endpoint.\n"; return 2; }
    auto token = juce::SystemStats::getEnvironmentVariable(tokenEnvironment, {});
    if (token.length() != 64 || token.containsAnyOf("\r\n")) { std::cerr << "The selected credential environment variable is missing or invalid.\n"; return 2; }
    Bridge bridge(endpoint, token); bridge.run(); return 0;
}
