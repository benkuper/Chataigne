#include "McpCore.h"
#include <iostream>
#include <functional>
using namespace organic::mcp;
static int checks = 0;
static void expect(bool value, const char* message)
{
    ++checks; if (!value) throw std::runtime_error(message);
}
static void rejects(const std::function<void()>& action, int code)
{
    try { action(); } catch (const Error& e) { expect(e.code == code, "wrong rejection code"); return; }
    throw std::runtime_error("action was not rejected");
}
int main()
{
    auto directory = juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("organic-mcp-test", {}, false);
    directory.createDirectory();
    try
    {
        auto store = directory.getChildFile("clients.json");
        Permissions permissions(store);
        auto client = permissions.create("untrusted display name", Access::edit, false, {directory.getFullPathName()});
        expect(client.second.length() == 64, "credential entropy length");
        expect(!store.loadFileAsString().contains(client.second), "credential leaked to store");
        Permissions restarted(store);
        auto grant = restarted.authenticate(client.second);
        expect(grant.allows(inspect | edit | files), "grant did not survive restart");
        expect(!grant.allows(advanced), "edit granted advanced execution");
        expect(grant.allowsFile(directory.getChildFile("new-project.noisette")), "approved file rejected");
        expect(!grant.allowsFile(directory.getSiblingFile("escape.noisette")), "folder escape allowed");
        Grant full; full.access = Access::full;
        expect(full.allows(inspect | control | edit | files | advanced), "full access incomplete");
        Registry registry; int executions = 0;
        auto nested = schema(object({{"rows", object({{"type", "array"}, {"items", schema(object({{"value", anySchema()}}), array({"value"}))}, {"maxItems", 3}})}}), array({"rows"}));
        checkSchema(nested);
        validate(nested, object({{"rows", array({object({{"value", array({1, 2, 3})}})})}}));
        expect(true, "nested object and array schema validation");
        rejects([&] { validate(schema(), array({})); }, -32602);
        rejects([&] { validate(nested, object({{"rows", object()}})); }, -32602);
        Operation operation;
        operation.name = "organic_test"; operation.description = "Test";
        operation.input = schema(object({{"amount", integerSchema(0, 10)}}), array({"amount"}));
        operation.output = schema(object({{"amount", integerSchema(0, 10)}}), array({"amount"}));
        operation.capabilities = edit;
        operation.execute = [&executions](const var& args, Context&) { ++executions; return args; };
        registry.add(operation);
        Context context; context.grant = grant;
        registry.call("organic_test", object({{"amount", 5}}), context);
        rejects([&] { registry.call("organic_test", object({{"amount", "5"}}), context); }, -32602);
        rejects([&] { registry.call("organic_test", object({{"amount", 11}}), context); }, -32602);
        rejects([&] { registry.call("organic_test", object({{"amount", 5}, {"script", "execute"}}), context); }, -32602);
        context.grant.access = Access::inspect;
        rejects([&] { registry.call("organic_test", object({{"amount", 5}}), context); }, -32003);
        expect(executions == 1, "validation ran after mutation");
        context.cancelled->store(true); rejects([&] { context.check(); }, -32800); context.cancelled->store(false);
        context.deadline = 0; rejects([&] { context.check(); }, -32002); context.deadline = juce::Time::getMillisecondCounterHiRes() + 30000;
        Protocol protocol(registry, permissions, "Test", "1.0");
        HttpRequest request; request.host = "127.0.0.1:42002"; request.authorization = "Bearer " + client.second;
        request.version = "2026-07-28"; request.rpcMethod = "tools/list";
        auto metadata = object({{"io.modelcontextprotocol/protocolVersion", request.version},
            {"io.modelcontextprotocol/clientInfo", object({{"name", "test"}, {"version", "1"}})},
            {"io.modelcontextprotocol/clientCapabilities", object()}});
        request.body = juce::JSON::toString(object({{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/list"}, {"params", object({{"_meta", metadata}})}}));
        auto modern = protocol.dispatch(request, context);
        expect(modern.status == 200 && modern.body["result"]["tools"].size() == 1 && modern.session.isEmpty(), "modern stateless list");
        request.origin = "https://evil.example"; expect(protocol.dispatch(request, context).status == 403, "origin protection"); request.origin = {};
        request.host = "evil.example:42002"; expect(protocol.dispatch(request, context).status == 403, "host protection"); request.host = "localhost:42002";
        request.rpcMethod = "ping"; expect((int) protocol.dispatch(request, context).body["error"]["code"] == -32020, "metadata mismatch protection");
        request.version = "1900-01-01"; expect((int) protocol.dispatch(request, context).body["error"]["code"] == -32022, "unsupported version error"); request.version = "2026-07-28";
        rejects([] { parseBounded(String::repeatedString("[", 33) + String::repeatedString("]", 33)); }, -32600);
        request.body = "["; expect((int) protocol.dispatch(request, context).body["error"]["code"] == -32700, "parse error");
        request.body = "[]"; expect((int) protocol.dispatch(request, context).body["error"]["code"] == -32600, "batch rejection");
        for (const auto* version : {"2025-11-25", "2025-06-18"})
        {
            request.version = {}; request.rpcMethod = {};
            request.body = juce::JSON::toString(object({{"jsonrpc", "2.0"}, {"id", 2}, {"method", "initialize"}, {"params", object({{"protocolVersion", version}})}}));
            auto init = protocol.dispatch(request, context);
            expect(init.status == 200 && init.body["result"]["protocolVersion"].toString() == version && init.session.isNotEmpty(), "legacy initialization");
            request.version = version; request.session = init.session;
            request.body = R"({"jsonrpc":"2.0","method":"notifications/initialized"})";
            expect(protocol.dispatch(request, context).status == 202, "legacy initialized notification");
            request.body = R"({"jsonrpc":"2.0","id":3,"method":"tools/list"})";
            expect(protocol.dispatch(request, context).status == 200, "legacy tool list");
            request.method = "DELETE"; expect(protocol.dispatch(request, context).status == 204, "session deletion"); request.method = "POST";
            expect(protocol.dispatch(request, context).status == 404, "deleted session accepted");
        }
        auto rotated = permissions.rotate(client.first.id);
        rejects([&] { permissions.authenticate(client.second); }, -32001);
        expect(permissions.authenticate(rotated).id == client.first.id, "rotation failed");
        permissions.revoke(client.first.id);
        rejects([&] { permissions.authenticate(rotated); }, -32001);
        expect(Permissions(store).list().isEmpty(), "revocation did not survive restart");
        std::cout << "Passed " << checks << " MCP core checks\n";
    }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; directory.deleteRecursively(); return 1; }
    directory.deleteRecursively(); return 0;
}
