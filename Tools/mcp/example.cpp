#include "McpServer.h"
#include <iostream>

// Independent of Chataigne: embeds the optional OrganicUI MCP foundation.
int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI runtime;
    using namespace organic::mcp;
    auto directory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("OrganicMcpExample");
    int port = 42003; bool createClient = false;
    for (int i = 1; i < argc; ++i)
    {
        String argument(argv[i]);
        if (argument == "--create-client") createClient = true;
        else if (argument == "--store" && i + 1 < argc) directory = juce::File(String(argv[++i]));
        else if (argument == "--port" && i + 1 < argc) port = String(argv[++i]).getIntValue();
        else { std::cerr << "Usage: organic_mcp_example [--create-client] [--store directory] [--port port]\n"; return 2; }
    }
    Permissions permissions(directory.getChildFile("clients.json"));
    if (createClient)
    {
        auto credential = permissions.create("Example client", Access::inspect);
        std::cout << "Bearer credential (shown once): " << credential.second << std::endl;
        return 0;
    }
    Registry registry;
    Operation op;
    op.name = "organic_example_inspect";
    op.description = "Inspect the independent example's current value.";
    op.output = schema(object({{"value", integerSchema(0, 100)}}), array({"value"}));
    op.execute = [](const var&, Context&) { return object({{"value", 42}}); };
    registry.add(op);
    registry.add(Resource{"organic://identity", "identity", "Application identity", [](Context&) { return object({{"name", "Organic MCP example"}}); }});
    registry.add(Resource{"organic://clock", "clock", "Observed wall clock for subscription examples", [](Context&) { return object({{"second", juce::Time::currentTimeMillis() / 1000}}); }});
    Server server(registry, permissions, "Organic MCP example", "1.0.0");
    auto result = server.start(port);
    if (result.failed()) { std::cerr << result.getErrorMessage() << std::endl; return 1; }
    std::cerr << "Listening at http://127.0.0.1:" << port << "/mcp; Ctrl+C to stop.\n";
    // The core/events-only macOS example has no NSApplication. Pump the CF/OS
    // message loop directly, which also works for console hosts on other systems.
    while (juce::MessageManager::getInstance()->runDispatchLoopUntil(1000)) {}
}
