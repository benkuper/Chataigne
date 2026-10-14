#include "../..//Modules/juce_organicui/assistant/AssistantCore.h"
#include <iostream>

namespace test
{
using namespace organic::assistant;
using organic::mcp::object;
using organic::mcp::array;
int checks = 0;
void require(bool condition, const char* label) { ++checks; if (!condition) throw std::runtime_error(label); }
template<class F> void rejects(F task) { bool rejected = false; try { task(); } catch (const organic::mcp::Error&) { rejected = true; } require(rejected, "Expected rejection"); }
String encodedJSON(const var& value) { auto json = juce::JSON::toString(value, true); return base64url(json.toRawUTF8(), json.getNumBytesAsUTF8()); }
juce::MemoryBlock bigEndian(juce::BigInteger number, size_t size = 0)
{
    auto bytes = number.toMemoryBlock(); auto result = juce::MemoryBlock(juce::jmax(size, bytes.getSize()), true);
    for (size_t i = 0; i < bytes.getSize(); ++i) ((char*) result.getData())[result.getSize() - i - 1] = ((char*) bytes.getData())[i];
    return result;
}
String numberEncoding(const String& hex)
{
    juce::BigInteger number; number.parseString(hex, 16); auto bytes = bigEndian(number); return base64url(bytes.getData(), bytes.getSize());
}
String sign(const var& claims, const juce::RSAKey& key, const String& algorithm = "RS256")
{
    auto data = encodedJSON(object({{"alg", algorithm}, {"kid", "test"}})) + "." + encodedJSON(claims);
    auto hash = juce::SHA256(data.toRawUTF8(), data.getNumBytesAsUTF8()).toHexString();
    String encoded = "0001";
    for (int i = 0; i < 202; ++i) encoded += "ff";
    encoded += "003031300d060960864801650304020105000420" + hash;
    juce::BigInteger signature; signature.parseString(encoded, 16); key.applyToValue(signature);
    auto bytes = bigEndian(signature, 256); return data + "." + base64url(bytes.getData(), bytes.getSize());
}
int run()
{
    try
    {
        require(pkceChallenge("dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk") == "E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM", "RFC7636 PKCE");
        rejects([] { decode64("unsafe="); });
        juce::RSAKey publicKey, privateKey;
        juce::BigInteger modulus;
        do { juce::RSAKey::createKeyPair(publicKey, privateKey, 2048); modulus.parseString(publicKey.toString().fromFirstOccurrenceOf(",", false, false), 16); } while (modulus.getHighestBit() < 2047);
        auto keyParts = juce::StringArray::fromTokens(publicKey.toString(), ",", "");
        auto jwks = object({{"keys", array({object({{"kid", "test"}, {"kty", "RSA"}, {"e", numberEncoding(keyParts[0])}, {"n", numberEncoding(keyParts[1])}})})}});
        auto claims = object({{"iss", "https://auth.openai.com"}, {"sub", "person"}, {"aud", "issued-client"}, {"iat", 999}, {"exp", 2000}, {"nonce", "nonce"}});
        auto token = sign(claims, privateKey);
        require(verifyIdentity(token, jwks, "issued-client", "nonce", 1000)["sub"].toString() == "person", "Valid RSA signed identity");
        rejects([&] { verifyIdentity(token, jwks, "different-client", "nonce", 1000); });
        rejects([&] { verifyIdentity(token, jwks, "issued-client", "wrong-nonce", 1000); });
        rejects([&] { verifyIdentity(token, jwks, "issued-client", "nonce", 2000); });
        rejects([&] { verifyIdentity(sign(claims, privateKey, "none"), jwks, "issued-client", "nonce", 1000); });
        auto tampered = token.substring(0, token.length() - 8) + "AAAAAAAA";
        rejects([&] { verifyIdentity(tampered, jwks, "issued-client", "nonce", 1000); });
        auto changed = claims.clone(); changed.getDynamicObject()->setProperty("iss", "https://other.example");
        rejects([&] { verifyIdentity(sign(changed, privateKey), jwks, "issued-client", "nonce", 1000); });
        auto arrayAudience = claims.clone(); arrayAudience.getDynamicObject()->setProperty("aud", array({"issued-client"}));
        require(verifyIdentity(sign(arrayAudience, privateKey), jwks, "issued-client", "nonce", 1000)["sub"].toString() == "person", "Single audience array does not require azp");
        arrayAudience.getDynamicObject()->setProperty("azp", "other-client");
        rejects([&] { verifyIdentity(sign(arrayAudience, privateKey), jwks, "issued-client", "nonce", 1000); });
        require(accountKey(claims, "one") != accountKey(claims, "two"), "Registrations remain distinct");
        auto prior = object({{"client_id", "issued"}, {"subject", "person"}, {"scope", "chatgpt.tokens.use.direct"}});
        auto response = object({{"access_token", "new-access"}, {"refresh_token", "rotated-refresh"}, {"token_type", "Bearer"}, {"expires_in", 3600}, {"earliest_refresh_at", 3000}});
        auto renewed = renewedAccount(prior, response, 1000);
        require(renewed["expires_at"] == var(4600.0) && renewed["refresh_token"].toString() == "rotated-refresh" && !prior.hasProperty("access_token"), "Refresh atomically replaces a copy");
        auto bad = response.clone(); bad.getDynamicObject()->setProperty("scope", "openid"); rejects([&] { renewedAccount(prior, bad, 1000); });
        bad = response.clone(); bad.getDynamicObject()->removeProperty("refresh_token"); rejects([&] { renewedAccount(prior, bad, 1000); });
        auto folder = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("assistant-test-" + juce::Uuid().toString());
        ProtectedStore store(folder); store.write("account", renewed); store.write("account", prior);
        require(store.read("account")["client_id"].toString() == "issued", "Protected atomic replacement");
        rejects([&] { store.read("../escape"); });
#if JUCE_WINDOWS
        require(!folder.getChildFile("account.private").loadFileAsString().contains("issued"), "Windows storage encrypted");
#endif
        folder.deleteRecursively();
        ResponseStream stream; String text;
        String events = String::fromUTF8("data: {\"type\":\"response.output_text.delta\",\"delta\":\"H\xc3\xa9llo\"}\r\n\r\ndata: {\"type\":\"response.completed\",\"response\":{\"output\":[]}}\n\n");
        auto utf8 = events.toUTF8();
        for (size_t i = 0; i < events.getNumBytesAsUTF8(); ++i) stream.push(utf8.getAddress() + i, 1, [&](const String& d) { text += d; });
        stream.finish(); require(text == String::fromUTF8("H\xc3\xa9llo"), "Incremental UTF8 SSE");
        ResponseStream toolStream;
        String toolEvents = "data: {\"type\":\"response.output_item.done\",\"output_index\":0,\"item\":{\"type\":\"function_call\",\"name\":\"organic_inspect\",\"call_id\":\"fixture\",\"arguments\":\"{}\"}}\n\ndata: {\"type\":\"response.completed\",\"response\":{\"output\":[]}}\n\n";
        toolStream.push(toolEvents.toRawUTF8(), toolEvents.getNumBytesAsUTF8(), [](const String&) {}); toolStream.finish();
        require(toolStream.output.size() == 1 && toolStream.output[0]["name"].toString() == "organic_inspect", "Retain streamed calls when terminal output is empty");
        ResponseStream partialStream; String terminalText;
        String partialEvents = "data: {\"type\":\"response.output_item.done\",\"output_index\":0,\"item\":{\"type\":\"function_call\",\"name\":\"organic_set_value\",\"call_id\":\"configure\",\"arguments\":\"{}\"}}\n\ndata: {\"type\":\"response.completed\",\"response\":{\"output\":[{\"type\":\"message\",\"role\":\"assistant\",\"content\":[{\"type\":\"output_text\",\"text\":\"Configured\"}]}]}}\n\n";
        partialStream.push(partialEvents.toRawUTF8(), partialEvents.getNumBytesAsUTF8(), [&](const String& d) { terminalText += d; }); partialStream.finish();
        require(partialStream.output.size() == 2 && partialStream.output[1]["call_id"].toString() == "configure", "Retain configuration calls with a partially populated terminal output");
        require(terminalText == "Configured", "Terminal message text appears when no text deltas were sent");
        rejects([] { ResponseStream incomplete; incomplete.finish(); });
        rejects([] { ResponseStream failed; String e = "data: {\"type\":\"response.failed\",\"response\":{\"error\":{\"code\":\"subscription_sharing_usage_limit_exceeded\"}}}\n\n"; failed.push(e.toRawUTF8(), e.getNumBytesAsUTF8(), [](auto&) {}); });
        rejects([] { ResponseStream incomplete; String e = "data: {\"type\":\"response.incomplete\"}\n\n"; incomplete.push(e.toRawUTF8(), e.getNumBytesAsUTF8(), [](auto&) {}); });
        auto retryable = [](const String& code)
        {
            try
            {
                ResponseStream failed; auto e = "data: {\"type\":\"response.failed\",\"response\":{\"error\":{\"code\":\"" + code + "\"}}}\n\n";
                failed.push(e.toRawUTF8(), e.getNumBytesAsUTF8(), [](auto&) {});
            }
            catch (const organic::mcp::Error& error) { return (bool) error.data["retryable"]; }
            return false;
        };
        require(retryable("server_error") && retryable("") && !retryable("subscription_sharing_usage_limit_exceeded"), "Retry transient and unclassified inference errors without retrying plan limits");
        for (auto payload : {object({{"type", "error"}, {"code", "server_error"}, {"message", "Temporary service failure"}}),
                             object({{"type", "response.failed"}, {"response", object({{"error", object({{"message", "Temporary service failure"}})}})}})})
        {
            bool diagnostic = false;
            try { ResponseStream failed; auto event = "data: " + juce::JSON::toString(payload, true) + "\n\n"; failed.push(event.toRawUTF8(), event.getNumBytesAsUTF8(), [](auto&) {}); }
            catch (const organic::mcp::Error& error) { diagnostic = String(error.what()).contains("Temporary service failure") && (bool) error.data["retryable"]; }
            require(diagnostic, "Preserve diagnostic messages from top-level and nested terminal errors");
        }
        require(usageError("server_error").contains("server_error"), "Inference failures preserve their diagnostic code");
        require(usageError("subscription_sharing_usage_unavailable").contains("unavailable"), "Eligibility recovery");
        require(reasoningEfforts("gpt-6.1-sol").contains("max") && !reasoningEfforts("gpt-6.1-sol").contains("none"), "Documented effort levels exclude unsupported no-reasoning values");
        require(reasoningEfforts("unknown-model").isEmpty(), "Unknown models keep the server default effort");
        require(actionableError("Access profile does not permit this operation", -32003).contains("Edit"), "Permission errors explain the panel setting to change");
        require(actionableError("Path is outside the approved folders", -32003).contains("Remember access"), "File-access recovery names the relevant settings");
        require(actionableError("Network unavailable").contains("Reconnect"), "Connection failures give recovery steps");
        require(actionableError("Unsupported effort").contains("Auto"), "Unsupported effort explains recovery");
        require(actionableError("Object changed since it was inspected", -32006).contains("Resume unfinished"), "Revision failures explain a user-facing recovery action");
        require(!actionableError("Object handle is stale", -32005).contains("current handle"), "Users are not asked to repair internal handles");
        require(actionableError("Incompatible target container").contains("compatible object"), "Target errors explain the failed binding and recovery");
        std::cout << "Passed " << checks << " assistant security and streaming checks\n"; return 0;
    }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
}
int main() { return test::run(); }
