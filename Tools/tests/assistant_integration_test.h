// Included in the native application regression: real OAuth callback, identity
// validation, protected persistence, UI, registry dispatch; only OpenAI is mocked.
#pragma once
namespace mcp_test
{
static void assistantChecks(const juce::File& directory)
{
#if ORGANICUI_USE_ASSISTANT
    using organic::assistant::ChatGptProvider;
    using organic::assistant::ResponseStream;
    using organic::assistant::base64url;
    juce::RSAKey publicKey, privateKey; juce::BigInteger modulus;
    do { juce::RSAKey::createKeyPair(publicKey, privateKey, 2048); modulus.parseString(publicKey.toString().fromFirstOccurrenceOf(",", false, false), 16); } while (modulus.getHighestBit() < 2047);
    auto keyParts = juce::StringArray::fromTokens(publicKey.toString(), ",", "");
    auto bigEndian = [](juce::BigInteger value, size_t size = 0)
    {
        auto bytes = value.toMemoryBlock(); juce::MemoryBlock result(juce::jmax(size, bytes.getSize()), true);
        for (size_t i = 0; i < bytes.getSize(); ++i) ((char*) result.getData())[result.getSize() - i - 1] = ((char*) bytes.getData())[i];
        return result;
    };
    auto encodeNumber = [&](const String& text) { juce::BigInteger value; value.parseString(text, 16); auto bytes = bigEndian(value); return base64url(bytes.getData(), bytes.getSize()); };
    auto jwks = object({{"keys", array({object({{"kid", "fixture"}, {"kty", "RSA"}, {"e", encodeNumber(keyParts[0])}, {"n", encodeNumber(keyParts[1])}})})}});
    String nonce, challenge, hostId, requestedClient, issuedClient = "issued-integration-client";
    std::thread browser;
    int renewals = 0, revocations = 0, edits = 0, rounds = 0;
    bool wrongIdentity = false, failure = false, waitForCancel = false, expired = true;
    juce::int64 generation = 1;
    auto jwt = [&]
    {
        auto now = juce::Time::getCurrentTime().toMilliseconds() / 1000.0;
        auto encode = [&](const var& v) { auto json = juce::JSON::toString(v, true); return base64url(json.toRawUTF8(), json.getNumBytesAsUTF8()); };
        auto text = encode(object({{"alg", "RS256"}, {"kid", "fixture"}})) + "." + encode(object({{"iss", "https://auth.openai.com"},
            {"sub", wrongIdentity ? "other-person" : "fixture-person"}, {"aud", issuedClient}, {"iat", now - 1}, {"exp", now + 3600}, {"nonce", nonce}, {"email", "fixture@example.test"}}));
        auto hash = juce::SHA256(text.toRawUTF8(), text.getNumBytesAsUTF8()).toHexString(); String encoded = "0001";
        for (int i = 0; i < 202; ++i) encoded += "ff";
        encoded += "003031300d060960864801650304020105000420" + hash;
        juce::BigInteger signature; signature.parseString(encoded, 16); privateKey.applyToValue(signature);
        auto bytes = bigEndian(signature, 256); return text + "." + base64url(bytes.getData(), bytes.getSize());
    };
    auto parameter = [](const juce::URL& url, const String& key) { auto index = url.getParameterNames().indexOf(key); return index < 0 ? String() : url.getParameterValues()[index]; };
    ChatGptProvider::HostServices services;
    services.openBrowser = [&](const juce::URL& url)
    {
        if (browser.joinable()) browser.join();
        nonce = parameter(url, "nonce"); challenge = parameter(url, "code_challenge");
        requestedClient = parameter(url, "client_id");
        auto nextHost = parameter(url, "ext_agent_host_id");
        if (!hostId.isEmpty()) check(hostId == nextHost, "OAuth stable host identity");
        hostId = nextHost;
        check(parameter(url, "code_challenge_method") == "S256" && !nonce.isEmpty(), "OAuth nonce and PKCE");
        auto redirect = parameter(url, "redirect_uri"), state = parameter(url, "state");
        browser = std::thread([redirect, state, issuedClient]
        {
            auto target = juce::URL(redirect).withParameter("state", state).withParameter("code", "fixture-code").withParameter("client_id", issuedClient);
            juce::WebInputStream response(target, false); response.withConnectionTimeout(3000).withNumRedirectsToFollow(0); response.connect(nullptr);
        });
        return true;
    };
    services.request = [&](const String& url, const String&, const String& body, const String&, bool,
        ResponseStream* stream, const std::function<void(const String&)>& delta, std::shared_ptr<std::atomic<bool>> cancelled) -> var
    {
        if (url.endsWith("jwks.json")) return jwks;
        if (url.endsWith("oauth/token"))
        {
            auto fields = juce::URL("https://fixture.test/?" + body);
            check(parameter(fields, "client_id") == issuedClient, "Token exchange uses issued client");
            if (parameter(fields, "grant_type") == "authorization_code")
                check(organic::assistant::pkceChallenge(parameter(fields, "code_verifier")) == challenge, "Token exchange PKCE matches browser");
            else ++renewals;
            return object({{"access_token", "fixture-access"}, {"refresh_token", "fixture-refresh-" + String(renewals)}, {"id_token", jwt()},
                {"token_type", "Bearer"}, {"expires_in", expired && renewals == 0 ? 1 : 3600}, {"scope", "openid chatgpt.tokens.use.direct"}, {"earliest_refresh_at", 0}});
        }
        if (url.endsWith("oauth/revoke")) { ++revocations; return object(); }
        if (url.endsWith("/models")) return object({{"models", array({object({{"visibility", "hidden"}, {"slug", "hidden"}}), object({{"visibility", "list"}, {"slug", "fixture-model"}, {"display_name", "Fixture model"}})})}});
        if (url.endsWith("/responses"))
        {
            auto payload = parseBounded(body);
            check(!(bool) payload["store"] && (bool) payload["stream"] && !payload.hasProperty("previous_response_id"), "Subscription preview request shape");
            check(payload["tools"][0]["type"].toString() == "namespace", "Local tools use namespace");
            if (waitForCancel) { while (!cancelled->load()) juce::Thread::sleep(5); throw Error(-32800, "Fixture cancelled"); }
            var event;
            if (failure) event = object({{"type", "response.failed"}, {"response", object({{"error", object({{"code", "subscription_sharing_usage_limit_exceeded"}})}})}});
            else if (rounds++ % 2 == 0) event = object({{"type", "response.completed"}, {"response", object({{"output", array({object({{"type", "function_call"}, {"name", "organic_fixture_edit"}, {"namespace", "chataigne"}, {"call_id", "call-" + String(rounds)}, {"arguments", "{}"}})})}})}});
            else event = object({{"type", "response.completed"}, {"response", object({{"output", array({object({{"type", "message"}, {"role", "assistant"}, {"content", array({object({{"type", "output_text"}, {"text", "Complete"}})})}})})}})}});
            auto encoded = "data: " + juce::JSON::toString(event, true) + "\n\n";
            stream->push(encoded.toRawUTF8(), encoded.getNumBytesAsUTF8(), delta); stream->finish(); return {};
        }
        throw std::runtime_error("Unexpected assistant endpoint");
    };
    Registry registry; Operation editOp; editOp.name = "organic_fixture_edit"; editOp.description = "Fixture edit"; editOp.capabilities = edit;
    editOp.execute = [&](const var&, Context&) { check(juce::MessageManager::getInstance()->isThisTheMessageThread(), "Assistant tool runs on application thread"); ++edits; return object(); }; registry.add(editOp);
    Permissions permissions(directory.getChildFile("assistant-clients.json"));
    auto assistantDirectory = directory.getChildFile("assistant");
    {
        ChatGptProvider provider(registry, permissions, assistantDirectory, "Chataigne", [&] { return generation; }, [] { return object({{"project", "Fixture"}}); }, services);
        auto wait = [&]
        {
            for (int i = 0; i < 400 && provider.view().busy; ++i) pumpMessages(25);
            check(!provider.view().busy, "Assistant completes within deadline");
            if (browser.joinable()) browser.join();
        };
        provider.signIn(true); wait();
        check(provider.view().accounts.getArray()->size() == 1 && provider.view().models.getArray()->size() == 1, "Verified account and filtered model catalog");
        check(renewals == 1 && provider.grant().access == Access::inspect, "Automatic renewal and initial Inspect grant");
        provider.send("Attempt edit", "fixture-model"); wait(); check(edits == 0 && provider.view().transcript.contains("-32003"), "Assistant cannot bypass Inspect grant");
        provider.setAccess(Access::full, true, {}); provider.send("Perform edit", "fixture-model"); wait(); check(edits == 1, "Remembered Full grant executes without app prompts");
        auto credential = provider.grant().id; permissions.revoke(credential);
        provider.send("Attempt revoked edit", "fixture-model"); wait(); check(edits == 1, "Assistant credential revocation enforced");
        failure = true; provider.send("Usage failure", "fixture-model"); wait(); check(provider.view().status.contains("usage limit"), "Midstream subscription limit visible"); failure = false;
        wrongIdentity = true; provider.signIn(false); wait(); check(provider.view().status.contains("differs"), "Returning sign-in identity mismatch rejected"); wrongIdentity = false;
        check(requestedClient == issuedClient && provider.view().accounts.getArray()->size() == 1, "Returning sign-in preserves registration");
        {
            organic::assistant::Controller controller(provider);
            check(!controller.includeInRecursiveSave && controller.prompt->parentContainer == &controller, "Conversation uses non-project OrganicUI controls");
            check(controller.models->parentContainer == &controller.settings && !controller.settings.getControllableByName("access_token"), "Account settings contain no raw tokens");
            controller.findIssues->trigger(); check(controller.prompt->stringValue().contains("diagnostics"), "Prompt suggestions are controllable triggers");
            organic::assistant::Panel panel("ChatGPT Assistant", controller); panel.setSize(660, 900); panel.resized();
            auto snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            auto preview = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries/mcp-assistant-panel.png");
            if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
            panel.setSize(320, 700); panel.resized();
            preview = preview.getSiblingFile("mcp-assistant-panel-narrow.png"); snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
        }
        provider.clearHistory(); check(provider.view().transcript.isEmpty(), "History deletion clears local transcript");
        waitForCancel = true; provider.send("Cancel pending inference", "fixture-model"); pumpMessages(100); provider.cancel(); waitForCancel = false;
        check(!provider.view().busy, "Cancellation joins pending network work");
        provider.signOut(); wait(); check(revocations == 1 && provider.view().models.getArray()->isEmpty(), "Sign-out revokes renewable session");
    }
    ChatGptProvider restored(registry, permissions, assistantDirectory, "Chataigne", [&] { return generation; }, [] { return object(); }, services);
    check(restored.view().accounts.getArray()->size() == 1 && !restored.view().active.isEmpty(), "Registration survives sign-out and restart");
    restored.refreshModels(); while (restored.view().busy) pumpMessages(25);
    check(restored.view().status.contains("sign in again"), "Signed-out credentials unavailable after restart");
    if (browser.joinable()) browser.join();
#endif
}
}
