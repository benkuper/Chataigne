#pragma once

#include <juce_core/juce_core.h>
#include <atomic>
#include <cstring>
#include <sstream>

// Chataigne's loopback endpoint sends length-delimited JSON or close-delimited
// SSE. Keep all socket operations on the request worker: cancellation only
// sets a flag, and bounded readiness waits let that worker unwind its own IO.
// This avoids native URL session callbacks racing with transport destruction.
class LoopbackHttpStream
{
public:
    LoopbackHttpStream(const juce::String& endpoint, bool events, const juce::String& headers, const juce::String& body)
        : port(juce::URL(endpoint).getPort())
    {
        request = (events ? "GET" : "POST") + juce::String(" /mcp HTTP/1.1\r\nHost: 127.0.0.1:") + juce::String(port)
            + "\r\n" + headers + "Content-Length: " + juce::String(events ? 0 : body.getNumBytesAsUTF8())
            + "\r\n\r\n" + (events ? juce::String() : body);
    }

    void cancel() { cancelled.store(true); }

    bool connect()
    {
        if (cancelled || !socket.connect("127.0.0.1", port, 5000)) return false;
        const auto deadline = juce::Time::getMillisecondCounterHiRes() + 5000;
        const auto bytes = request.toRawUTF8();
        const int size = request.getNumBytesAsUTF8();
        for (int sent = 0; sent < size;)
        {
            if (!ready(false, deadline)) return false;
            const int count = socket.write(bytes + sent, juce::jmin(4096, size - sent));
            if (count <= 0) { failed = true; return false; }
            sent += count;
        }
        request.clear(); // Do not retain the credential after sending it.
        for (;;)
        {
            const auto end = pending.find("\r\n\r\n");
            if (end != std::string::npos)
            {
                if (end > 65536) return false;
                const auto lines = juce::StringArray::fromLines(juce::String::fromUTF8(pending.data(), static_cast<int>(end)));
                std::istringstream first(lines[0].toStdString());
                std::string protocol;
                first >> protocol >> status;
                if ((protocol != "HTTP/1.1" && protocol != "HTTP/1.0") || status < 200 || status > 599) return false;
                for (int i = 1; i < lines.size(); ++i)
                {
                    const int colon = lines[i].indexOfChar(':');
                    if (colon <= 0) return false;
                    responseHeaders.set(lines[i].substring(0, colon).trim(), lines[i].substring(colon + 1).trim());
                }
                // The selected Chataigne server does not use transfer encodings.
                if (responseHeaders["Transfer-Encoding"].isNotEmpty()) return false;
                if (responseHeaders.containsKey("Content-Length"))
                {
                    const auto length = responseHeaders["Content-Length"];
                    if (length.isEmpty() || !length.containsOnly("0123456789") || length.length() > 18) return false;
                    totalLength = length.getLargeIntValue();
                }
                pending.erase(0, end + 4);
                exhausted = totalLength == 0;
                return !cancelled;
            }
            if (pending.size() > 65536 || !ready(true, deadline)) return false;
            char buffer[8192];
            const int count = socket.read(buffer, sizeof(buffer), false);
            if (count <= 0) return false;
            pending.append(buffer, static_cast<size_t>(count));
        }
    }

    int read(void* destination, int maximum)
    {
        if (isExhausted() || isError() || maximum <= 0) return 0;
        if (offset == pending.size())
        {
            // SSE can be idle indefinitely; cancellation is still observed at
            // least every 20ms. Ordinary response bodies have a bounded wait.
            const auto deadline = responseHeaders["Content-Type"].contains("text/event-stream") ? 0
                : juce::Time::getMillisecondCounterHiRes() + 5000;
            if (!ready(true, deadline)) return 0;
            char buffer[8192];
            const int count = socket.read(buffer, sizeof(buffer), false);
            if (count <= 0) { exhausted = true; failed = count < 0; return 0; }
            pending.assign(buffer, static_cast<size_t>(count));
            offset = 0;
        }
        auto count = juce::jmin(static_cast<size_t>(maximum), pending.size() - offset);
        if (totalLength >= 0) count = juce::jmin(count, static_cast<size_t>(totalLength - position));
        std::memcpy(destination, pending.data() + offset, count);
        offset += count;
        position += static_cast<juce::int64>(count);
        if (totalLength >= 0 && position == totalLength) exhausted = true;
        return static_cast<int>(count);
    }

    bool isExhausted() const { return exhausted || cancelled; }
    bool isError() const { return failed || cancelled; }
    int getStatusCode() const { return status; }
    juce::StringPairArray getResponseHeaders() const { return responseHeaders; }
    juce::int64 getTotalLength() const { return totalLength; }
    juce::int64 getPosition() const { return position; }

private:
    bool ready(bool reading, double deadline)
    {
        while (!cancelled)
        {
            if (deadline > 0 && juce::Time::getMillisecondCounterHiRes() >= deadline) break;
            const int result = socket.waitUntilReady(reading, 20);
            if (result > 0) return true;
            if (result < 0) break;
        }
        failed = true;
        return false;
    }

    juce::StreamingSocket socket;
    const int port;
    juce::String request;
    juce::StringPairArray responseHeaders;
    std::string pending;
    size_t offset = 0;
    juce::int64 totalLength = -1, position = 0;
    int status = 0;
    bool failed = false, exhausted = false;
    std::atomic<bool> cancelled { false };
};
