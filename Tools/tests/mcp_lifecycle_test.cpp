#include "McpServer.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace organic::mcp;

static void require(bool condition, const char* message) { if (!condition) throw std::runtime_error(message); }
static void pump(int milliseconds)
{
    juce::MessageManager::getInstance()->runDispatchLoopUntil(milliseconds);
}
int main()
{
    juce::ScopedJuceInitialiser_GUI runtime;
    auto directory = juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("mcp-lifecycle", {}, false);
    directory.createDirectory();
    try
    {
        Permissions permissions(directory.getChildFile("clients.json")); auto token = permissions.create("Lifecycle test", Access::inspect).second;
        Registry registry; int executions = 0;
        Operation op; op.name = "inspect_pending"; op.description = "Lifecycle fixture";
        op.execute = [&](const var&, Context&) { ++executions; return object(); }; registry.add(op);
        juce::Array<var> uris;
        for (int index = 0; index < 32; ++index)
        {
            auto uri = "organic://slow/" + String(index) + "/" + String::repeatedString("x", 30000); uris.add(uri);
            registry.add(Resource{uri, "slow", "Backpressure fixture", [](Context&) { return object({{"tick", juce::Time::currentTimeMillis()}}); }});
        }
        Server server(registry, permissions, "Lifecycle test", "1"); int port = 0;
        for (int attempt = 0; attempt < 20 && !server.isRunning(); ++attempt)
        { port = 43000 + juce::Random::getSystemRandom().nextInt(20000); server.start(port); }
        require(server.isRunning(), "Could not reserve lifecycle listener");
        std::cerr << "Lifecycle: queue bounds\n" << std::flush;
        auto connect = [&](const String& method, var params)
        {
            params.getDynamicObject()->setProperty("_meta", object({{"io.modelcontextprotocol/protocolVersion", "2026-07-28"},
                {"io.modelcontextprotocol/clientInfo", object({{"name", "lifecycle"}, {"version", "1"}})}, {"io.modelcontextprotocol/clientCapabilities", object()}}));
            auto body = juce::JSON::toString(object({{"jsonrpc", "2.0"}, {"id", 1}, {"method", method}, {"params", params}}), true);
            auto request = "POST /mcp HTTP/1.1\r\nHost: 127.0.0.1:" + String(port) + "\r\nAuthorization: Bearer " + token
                + "\r\nContent-Type: application/json\r\nAccept: application/json, text/event-stream\r\nMCP-Protocol-Version: 2026-07-28\r\nMcp-Method: " + method;
            if (method == "tools/call") request += "\r\nMcp-Name: inspect_pending";
            request += "\r\nContent-Length: " + String(body.getNumBytesAsUTF8()) + "\r\nConnection: close\r\n\r\n" + body;
            auto socket = std::make_unique<juce::StreamingSocket>(); require(socket->connect("127.0.0.1", port, 1000), "Client connect failed");
            require(socket->write(request.toRawUTF8(), (int) request.getNumBytesAsUTF8()) == (int) request.getNumBytesAsUTF8(), "Client write failed"); return socket;
        };
        auto read = [](juce::StreamingSocket& socket)
        {
            require(socket.waitUntilReady(true, 2000) == 1, "Response timed out"); char bytes[4096]; auto count = socket.read(bytes, sizeof(bytes), false);
            return count > 0 ? String::fromUTF8(bytes, count) : String();
        };
        std::vector<std::unique_ptr<juce::StreamingSocket>> queued;
        for (int index = 0; index < 64; ++index) queued.push_back(connect("tools/call", object({{"name", "inspect_pending"}, {"arguments", object()}})));
        juce::Thread::sleep(100);
        auto overflow = connect("ping", object()); require(read(*overflow).contains("429"), "Queue overflow must reject before model dispatch");
        server.invalidateDocument(); pump(300);
        for (auto& socket : queued) require(read(*socket).contains("409"), "Queued request survived project replacement");
        require(executions == 0, "Stale queued work reached the provider"); queued.clear();
        std::cerr << "Lifecycle: slow subscriber\n" << std::flush;

        auto slow = connect("subscriptions/listen", object({{"notifications", object({{"resourceSubscriptions", uris}})}}));
        // Deliberately leave approximately 1 MiB notifications unread until the
        // peer window fills. Other clients must continue to receive service.
        pump(8000);
        auto ping = connect("ping", object()); pump(100); require(read(*ping).contains("200"), "Slow subscriber blocked another client");
        int received = 0; bool closed = false; char bytes[16384]; auto until = juce::Time::getMillisecondCounterHiRes() + 2000;
        while (juce::Time::getMillisecondCounterHiRes() < until && received < 32 * 1024 * 1024)
        {
            auto ready = slow->waitUntilReady(true, 10); if (ready < 0) { closed = true; break; } if (ready == 0) { pump(1); continue; }
            auto count = slow->read(bytes, sizeof(bytes), false); if (count <= 0) { closed = true; break; } received += count;
        }
        require(closed, "Stalled streaming socket was not cancelled");
        std::cerr << "Lifecycle: pending shutdown\n" << std::flush;

        auto pending = connect("tools/call", object({{"name", "inspect_pending"}, {"arguments", object()}})); juce::Thread::sleep(80);
        auto stopStart = juce::Time::getMillisecondCounterHiRes(); server.stop(); pump(50);
        std::cerr << "Lifecycle: stopped\n" << std::flush;
        require(!server.isRunning() && executions == 0 && juce::Time::getMillisecondCounterHiRes() - stopStart < 2000,
            "Shutdown did not reject pending work promptly");
        std::cout << "Passed queue bounds, queued-generation invalidation, real slow-subscriber backpressure, concurrent service and pending-work shutdown\n";
    }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; directory.deleteRecursively(); return 1; }
    directory.deleteRecursively(); return 0;
}
