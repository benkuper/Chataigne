#include <servus/servus.h>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/error.h>
#include <avahi-common/simple-watch.h>
#include <dlfcn.h>
#include <cstdlib>
#include <iostream>
#include <string>

// Interpose only the daemon connection and event injection. Polling uses the
// real Avahi implementation, including the assertion after STATE_QUIT.
namespace
{
std::string scenario;
int polls = 0;
int clientToken, browserToken;
AvahiClientCallback clientCallback = nullptr;
void* clientData = nullptr;
AvahiServiceBrowserCallback browserCallback = nullptr;
void* browserData = nullptr;

void require(bool condition, const char* message)
{
    if (!condition)
    {
        std::cerr << scenario << ": " << message << '\n';
        std::exit(1);
    }
}
}

extern "C" AvahiClient* avahi_client_new(const AvahiPoll*, AvahiClientFlags,
    AvahiClientCallback callback, void* data, int*)
{
    clientCallback = callback;
    clientData = data;
    auto* client = reinterpret_cast<AvahiClient*>(&clientToken);
    callback(client, AVAHI_CLIENT_CONNECTING, data);
    return client;
}

extern "C" void avahi_client_free(AvahiClient*) {}
extern "C" int avahi_client_errno(AvahiClient*) { return AVAHI_ERR_DISCONNECTED; }

extern "C" AvahiServiceBrowser* avahi_service_browser_new(AvahiClient*,
    AvahiIfIndex, AvahiProtocol, const char*, const char*, AvahiLookupFlags,
    AvahiServiceBrowserCallback callback, void* data)
{
    browserCallback = callback;
    browserData = data;
    return reinterpret_cast<AvahiServiceBrowser*>(&browserToken);
}

extern "C" int avahi_service_browser_free(AvahiServiceBrowser*) { return 0; }

extern "C" AvahiServiceResolver* avahi_service_resolver_new(AvahiClient*,
    AvahiIfIndex, AvahiProtocol, const char*, const char*, const char*,
    AvahiProtocol, AvahiLookupFlags, AvahiServiceResolverCallback, void*)
{
    return nullptr;
}

static void injectFailure(AvahiTimeout* timer, void* data)
{
    if (scenario == "client-failure" || scenario == "registering")
        clientCallback(reinterpret_cast<AvahiClient*>(&clientToken),
            scenario == "registering" ? AVAHI_CLIENT_S_REGISTERING : AVAHI_CLIENT_FAILURE,
            clientData);
    else
        browserCallback(reinterpret_cast<AvahiServiceBrowser*>(&browserToken),
            AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC,
            scenario == "browser-failure" ? AVAHI_BROWSER_FAILURE : AVAHI_BROWSER_NEW,
            "test", "_osc._udp", "local", static_cast<AvahiLookupResultFlags>(0), browserData);
    avahi_simple_poll_get(static_cast<AvahiSimplePoll*>(data))->timeout_free(timer);
}

extern "C" int avahi_simple_poll_iterate(AvahiSimplePoll* poll, int timeout)
{
    ++polls;
    if (scenario == "poll-error") return -1;
    if (polls == 1 && scenario != "success")
    {
        if (scenario == "client-failure" || scenario == "registering"
            || scenario == "browser-failure" || scenario == "resolver-failure")
        {
            // Dispatch the failure inside the real iteration, which returns 0
            // even though the callback quits the poll. Servus must latch it.
            const auto* api = avahi_simple_poll_get(poll);
            timeval immediate = {};
            require(api->timeout_new(api, &immediate, injectFailure, poll) != nullptr,
                "could not schedule failure callback");
        }
        else
            avahi_simple_poll_quit(poll);
    }
    using Iterate = int (*)(AvahiSimplePoll*, int);
    static auto realIterate = reinterpret_cast<Iterate>(dlsym(RTLD_NEXT, "avahi_simple_poll_iterate"));
    require(realIterate != nullptr, "could not find real Avahi poll");
    return realIterate(poll, timeout);
}

int main(int argc, char** argv)
{
    require(argc == 2, "expected a scenario");
    scenario = argv[1];
    servus::Servus service("_osc._udp");
    if (scenario == "announcement-error")
    {
        require(service.announce(9000, "test") == servus::Servus::Result::POLL_ERROR,
            "announcement did not report poll failure");
        require(service.announce(9000, "test") == servus::Servus::Result::POLL_ERROR,
            "repeated announcement reused failed poll");
    }
    else
    {
        require(!!service.beginBrowsing(servus::Servus::IF_ALL), "could not begin browsing");
        if (scenario == "success")
        {
            require(!!service.browse(0), "healthy polling failed");
            require(!!service.browse(0), "healthy polling could not continue");
            require(polls == 2, "unexpected healthy poll count");
            std::cout << "PASS " << scenario << '\n';
            return 0;
        }
        require(service.browse(1000) == servus::Servus::Result::POLL_ERROR,
            "browse did not report terminal poll failure");
        require(service.browse(1000) == servus::Servus::Result::POLL_ERROR,
            "repeated browse reused failed poll");
        require(service.announce(9000, "test") == servus::Servus::Result::POLL_ERROR,
            "announcement reused failed poll");
    }
    service.endBrowsing();
    require(service.beginBrowsing(servus::Servus::IF_ALL) == servus::Servus::Result::POLL_ERROR,
        "browser restarted on failed poll");
    require(polls == 1, "terminal poll was iterated more than once");
    std::cout << "PASS " << scenario << '\n';
}
