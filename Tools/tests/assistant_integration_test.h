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
    int renewals = 0, revocations = 0, edits = 0, rounds = 0, toolsRemaining = -1, browserOpens = 0, inferences = 0;
    bool wrongIdentity = false, failure = false, waitForCancel = false, expired = true, modelNetworkFailure = false;
    bool prematureFinish = false;
    int transientFailures = 0, transientAttempts = 0;
    bool invalidCompletion = false;
    String expectedEffort;
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
        ++browserOpens;
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
        if (url.endsWith("/models"))
        {
            if (modelNetworkFailure) throw Error(-32030, "Network unavailable. Reconnect to try again.");
            return object({{"models", array({object({{"visibility", "hidden"}, {"slug", "hidden"}}), object({{"visibility", "list"}, {"slug", "gpt-6.1-sol"}, {"display_name", "GPT-6.1 Sol"}})})}});
        }
        if (url.endsWith("/responses"))
        {
            auto payload = parseBounded(body);
            check(!(bool) payload["store"] && (bool) payload["stream"] && !payload.hasProperty("previous_response_id"), "Subscription preview request shape");
            check(payload["tools"][0]["type"].toString() == "namespace", "Local tools use namespace");
            check(expectedEffort.isEmpty() ? !payload.hasProperty("reasoning") : payload["reasoning"]["effort"].toString() == expectedEffort, "Selected effort is sent on every inference round; Auto omits the override");
            if (waitForCancel) { while (!cancelled->load()) juce::Thread::sleep(5); throw Error(-32800, "Fixture cancelled"); }
            var event;
            if (failure) event = object({{"type", "response.failed"}, {"response", object({{"error", object({{"code", "subscription_sharing_usage_limit_exceeded"}})}})}});
            else if (transientFailures > 0 && payload["input"][payload["input"].size() - 1]["type"].toString() == "function_call_output")
            {
                --transientFailures; ++transientAttempts;
                auto interrupted = "data: " + juce::JSON::toString(object({{"type", "response.output_item.done"}, {"output_index", 0},
                    {"item", object({{"type", "function_call"}, {"name", "organic_fixture_edit"}, {"namespace", "chataigne"}, {"call_id", "unconfirmed"}, {"arguments", "{}"}})}}), true) + "\n\n";
                stream->push(interrupted.toRawUTF8(), interrupted.getNumBytesAsUTF8(), delta);
                event = object({{"type", "response.failed"}, {"response", object({{"error", object({{"code", "server_error"}})}})}});
            }
            else
            {
                ++inferences; bool terminalPlan = false, hasPlan = false, denied = false;
                for (auto item : *payload["input"].getArray())
                {
                    if (item["role"].toString() == "user") { terminalPlan = hasPlan = denied = false; }
                    if (item["type"].toString() == "function_call_output")
                    {
                        auto result = juce::JSON::parse(item["output"].toString());
                        denied |= (int) result["error"] == -32003 || (int) result["error"] == -32001;
                        if (result["tasks"].size() > 0) { hasPlan = true; terminalPlan = result["tasks"][0]["status"].toString() != "pending"; }
                    }
                }
                auto invoke = [&](const String& name, const var& args, const String& id)
                { return object({{"type", "function_call"}, {"name", name}, {"namespace", "chataigne"}, {"call_id", id}, {"arguments", juce::JSON::toString(args, true)}}); };
                auto task = [&](const String& status, const String& verification = String())
                { return object({{"tasks", array({object({{"id", "workflow"}, {"description", "Complete the requested workflow and verify configuration"}, {"status", status},
                    {"evidence", status == "blocked" ? "Local edit permission was denied" : "Final configuration inspected"}, {"verifiedBy", verification.isEmpty() ? array({}) : array({verification})}})})}}); };
                var output = array({}); auto id = "fixture-" + String(inferences);
                if (terminalPlan) output.append(object({{"type", "message"}, {"role", "assistant"}, {"content", array({object({{"type", "output_text"}, {"text", "Complete"}})})}}));
                else if (toolsRemaining >= 0 ? toolsRemaining-- > 0 : rounds++ % 2 == 0)
                {
                    if (!hasPlan) output.append(invoke("assistant_task", task("pending"), "plan-" + id));
                    output.append(invoke("organic_fixture_edit", object(), id));
                }
                else if (prematureFinish)
                {
                    prematureFinish = false;
                    output.append(object({{"type", "message"}, {"role", "assistant"}, {"content", array({object({{"type", "output_text"}, {"text", "Some work is still unfinished"}})})}}));
                }
                else if (denied) output.append(invoke("assistant_task", task("blocked"), "blocked-" + id));
                else if (invalidCompletion)
                {
                    invalidCompletion = false;
                    output.append(invoke("assistant_task", task("completed", "invented-inspection"), "invalid-" + id));
                }
                else
                {
                    output.append(invoke("organic_fixture_inspect", object(), "verify-" + id));
                    output.append(invoke("assistant_task", task("completed", "verify-" + id), "done-" + id));
                }
                event = object({{"type", "response.completed"}, {"response", object({{"output", output}})}});
            }
            auto encoded = "data: " + juce::JSON::toString(event, true) + "\n\n";
            stream->push(encoded.toRawUTF8(), encoded.getNumBytesAsUTF8(), delta); stream->finish(); return {};
        }
        throw std::runtime_error("Unexpected assistant endpoint");
    };
    Registry registry; Operation editOp; editOp.name = "organic_fixture_edit"; editOp.description = "Fixture edit"; editOp.capabilities = edit;
    editOp.execute = [&](const var&, Context&) { check(juce::MessageManager::getInstance()->isThisTheMessageThread(), "Assistant tool runs on application thread"); ++edits; return object(); }; registry.add(editOp);
    Operation inspectOp; inspectOp.name = "organic_fixture_inspect"; inspectOp.description = "Inspect the final configuration";
    inspectOp.output = openObjectSchema();
    inspectOp.execute = [&](const var&, Context&) { return object({{"edits", edits}}); }; registry.add(inspectOp);
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
        provider.send("Attempt edit", "gpt-6.1-sol"); wait(); check(edits == 0 && provider.view().transcript.contains("-32003"), "Assistant cannot bypass Inspect grant");
        provider.setAccess(Access::full, true, {}); provider.send("Perform edit", "gpt-6.1-sol"); wait();
        if (edits != 1) std::cerr << "Fixture edits=" << edits << ", status=" << provider.view().status << "\n" << provider.view().transcript << '\n';
        check(edits == 1, "Remembered Full grant executes without app prompts");
        toolsRemaining = 16; provider.send("Complete a longer workflow including configuration", "gpt-6.1-sol"); wait();
        check(edits == 17 && provider.view().activity == organic::assistant::Activity::completed, "Long authoring continues beyond twelve tool rounds");
        toolsRemaining = 100; provider.send("Complete a workflow beyond the old round limit", "gpt-6.1-sol"); wait();
        check(edits == 117 && provider.view().activity == organic::assistant::Activity::completed, "Authoring does not require user continuation at 96 rounds");
        toolsRemaining = 1; prematureFinish = true; auto beforeRecovery = inferences;
        provider.send("Finish all configuration without asking me to continue", "gpt-6.1-sol"); wait();
        check(inferences >= beforeRecovery + 4 && provider.view().activity == organic::assistant::Activity::completed, "Premature final response automatically continues pending work and verifies it");
        toolsRemaining = -1;
        transientFailures = 2; auto editsBeforeRetry = edits;
        provider.send("Finish despite a temporary inference failure", "gpt-6.1-sol"); wait();
        check(transientAttempts == 2 && edits == editsBeforeRetry + 1 && provider.view().activity == organic::assistant::Activity::completed, "Transient inference failures after an edit retry without replaying it or executing unconfirmed calls");
        invalidCompletion = true; provider.send("Verify before claiming completion", "gpt-6.1-sol"); wait();
        check(provider.view().transcript.contains("verifiedBy must reference") && provider.view().activity == organic::assistant::Activity::completed, "Invented verification evidence is rejected and repaired within the same turn");
        check(!provider.view().transcript.contains("Some work is still unfinished"), "Premature closing text is not presented as the final answer");
        auto previousConversation = provider.view().conversation, previousTranscript = provider.view().transcript;
        provider.newConversation(); check(provider.view().transcript.isEmpty() && provider.view().conversation != previousConversation, "New conversation retains previous history");
        provider.send("Second conversation", "gpt-6.1-sol"); wait();
        check(provider.view().conversations.size() == 2, "Conversations have separate saved records");
        provider.selectConversation(previousConversation); check(provider.view().transcript == previousTranscript, "History restores full context and transcript");
        {
            auto count = browserOpens;
            organic::assistant::ProtectedStore storage(assistantDirectory);
            auto savedAccounts = storage.read("accounts");
            for (auto& account : *savedAccounts["records"].getArray()) account.getDynamicObject()->setProperty("expires_at", 1.0);
            storage.write("accounts", savedAccounts); auto renewalsBeforeRestart = renewals;
            modelNetworkFailure = true;
            ChatGptProvider restarted(registry, permissions, assistantDirectory, "Chataigne", [&] { return generation; }, [] { return object(); }, services);
            while (restarted.view().busy) pumpMessages(25);
            check(renewals == renewalsBeforeRestart + 1, "Startup renews expired access using the saved refresh token");
            check(restarted.view().signedIn && restarted.view().models.size() == 0 && browserOpens == count, "Offline startup retains the session without reopening browser login");
            modelNetworkFailure = false; restarted.refreshModels(); while (restarted.view().busy) pumpMessages(25);
            check(restarted.view().signedIn && restarted.view().models.size() == 1 && browserOpens == count, "Startup restores signed-in models without browser authorization");
            check(restarted.view().conversation == previousConversation && restarted.view().transcript == previousTranscript, "Selected history survives restart");
        }
        auto credential = provider.grant().id; permissions.revoke(credential); auto editsBeforeRevocation = edits;
        provider.send("Attempt revoked edit", "gpt-6.1-sol"); wait(); check(edits == editsBeforeRevocation, "Assistant credential revocation enforced");
        check(provider.view().activity == organic::assistant::Activity::failed, "Failed project actions cannot report successful completion");
        failure = true; provider.send("Usage failure", "gpt-6.1-sol"); wait(); check(provider.view().status.contains("usage limit"), "Midstream subscription limit visible"); failure = false;
        wrongIdentity = true; provider.signIn(false); wait(); check(provider.view().status.contains("differs"), "Returning sign-in identity mismatch rejected"); wrongIdentity = false;
        check(requestedClient == issuedClient && provider.view().accounts.getArray()->size() == 1, "Returning sign-in preserves registration");
        provider.signIn(false); wait();
        {
            organic::assistant::Controller controller(provider);
            check(!controller.includeInRecursiveSave && controller.prompt->parentContainer == &controller, "Conversation uses non-project OrganicUI controls");
            check(controller.models->parentContainer == &controller.settings && !controller.settings.getControllableByName("access_token"), "Account settings contain no raw tokens");
            controller.access->setValueWithData((int) Access::edit); check(provider.grant().access == Access::edit, "Panel access changes are remembered immediately");
            controller.findIssues->trigger(); check(controller.prompt->stringValue().contains("diagnostics"), "Prompt suggestions are controllable triggers");
            organic::assistant::Panel panel("ChatGPT Assistant", controller); panel.setVisible(true); panel.setSize(660, 900); panel.resized();
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.effort; }) != nullptr, "Effort picker is in the panel");
            expectedEffort = "high"; controller.effort->setValueWithData("high"); controller.prompt->setValue("Complete an effort-controlled request"); controller.send->trigger(); wait();
            expectedEffort.clear(); controller.effort->setValueWithData(""); pumpMessages(250);
            rejects([&] { provider.send("Invalid effort", "gpt-6.1-sol", "none"); }, -32602);
            check(!findComponent<juce::TextEditor>(panel, [](const auto& editor) { return editor.getName() == "Conversation message"; }), "Messages use styled rendering instead of text editor controls");
            auto* messages = findComponent<juce::Component>(panel, [](const auto& component) { return component.getName() == "Conversation messages"; });
            auto* messageCopy = findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "Copy message"; });
            auto* copyAll = findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "Copy conversation"; });
            check(messages && messageCopy && !messageCopy->isVisible() && copyAll && copyAll->isVisible(), "Per-message copy is hidden until hover; copy conversation stays in the toolbar");
            auto pointerEvent = [&](juce::Point<float> point)
            {
                return juce::MouseEvent(juce::Desktop::getInstance().getMainMouseSource(), point, {}, 0, 0, 0, 0, 0,
                    messages, messages, juce::Time::getCurrentTime(), point, juce::Time::getCurrentTime(), 0, false);
            };
            messages->mouseMove(pointerEvent({20, 20}));
            check(messageCopy->isVisible(), "Hovering a message reveals its copy icon");
            auto copyPoint = messages->getLocalPoint(messageCopy, messageCopy->getLocalBounds().getCentre());
            messages->mouseExit(pointerEvent(copyPoint.toFloat()));
            check(messageCopy->isVisible() && messages->getComponentAt(copyPoint) == messageCopy, "Moving onto the copy icon keeps it visible and clickable");
            auto savedClipboard = juce::SystemClipboard::getTextFromClipboard();
            messageCopy->triggerClick(); pumpMessages(250);
            check(juce::SystemClipboard::getTextFromClipboard() == "Attempt edit", "Bubble copy copies only that message");
            messages->mouseMove(pointerEvent({-1, -1}));
            check(!messageCopy->isVisible(), "Leaving a message hides its copy icon");
            copyAll->triggerClick(); pumpMessages(250);
            check(juce::SystemClipboard::getTextFromClipboard() == panel.conversationText(), "Toolbar copy copies the entire readable conversation and troubleshooting context");
            juce::SystemClipboard::copyTextToClipboard(savedClipboard);
            check(panel.conversationText().contains("Model: GPT-6.1 Sol") && panel.conversationText().contains("What to try:"), "Copyable conversation includes model, readable errors and recovery instructions");
            check(!panel.conversationText().contains("fixture-access") && !panel.conversationText().contains("[Tool:"), "Conversation export contains no credentials or raw internal tool payloads");
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.access; }) != nullptr, "Access picker is available directly in the panel");
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.models; }) != nullptr, "Model picker is available directly in the panel");
            auto checkSelectorRow = [&]
            {
                auto modelBounds = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.models; })->getBounds();
                auto effortBounds = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.effort; })->getBounds();
                auto accessBounds = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.access; })->getBounds();
                check(modelBounds.getY() == effortBounds.getY() && effortBounds.getY() == accessBounds.getY(), "Model, effort and access remain on the same row");
                check(modelBounds.getWidth() > 40 && effortBounds.getWidth() > 40 && accessBounds.getWidth() > 40
                    && modelBounds.getRight() < effortBounds.getX() && effortBounds.getRight() < accessBounds.getX()
                    && panel.getLocalBounds().contains(accessBounds), "All three selectors fit without overlap");
            };
            checkSelectorRow();
            auto snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            auto preview = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries/mcp-assistant-panel.png");
            if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
            auto* historyButton = findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "Conversation history"; });
            check(historyButton != nullptr, "History uses a compact toolbar icon"); historyButton->triggerClick(); pumpMessages(250);
            check(!findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.models; })->isVisible(), "History replaces the conversation view");
            snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            if (auto output = preview.getSiblingFile("mcp-assistant-history.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
            auto click = [&](juce::Component& target, juce::Point<int> point)
            {
                auto event = juce::MouseEvent(juce::Desktop::getInstance().getMainMouseSource(), point.toFloat(), juce::ModifierKeys(juce::ModifierKeys::leftButtonModifier), 1, 0, 0, 0, 0,
                    &target, &target, juce::Time::getCurrentTime(), point.toFloat(), juce::Time::getCurrentTime(), 1, false);
                target.mouseDown(event); target.mouseUp(event); pumpMessages(250);
            };
            auto* historyList = findComponent<juce::Component>(panel, [](const auto& component) { return component.getName() == "Conversation history list"; });
            check(historyList && panel.getComponentAt(panel.getLocalPoint(historyList, juce::Point<int>(20, 20))) == historyList, "History rows receive actual pointer hits");
            auto targetConversation = provider.view().conversations[0]["id"].toString();
            click(*historyList, {20, 20}); check(provider.view().conversation == targetConversation, "Clicking a specific history row selects it");
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.models; })->isVisible(), "Selecting history returns to the conversation");
            provider.selectConversation(previousConversation); pumpMessages(250);
            historyButton->triggerClick(); pumpMessages(250);
            waitForCancel = true; provider.send("Running request before history selection", "gpt-6.1-sol"); pumpMessages(250);
            auto* stopButton = findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "Stop"; });
            check(stopButton && stopButton->isVisible(), "Stop remains accessible in history during a request");
            click(*historyList, {20, 20}); waitForCancel = false;
            check(!provider.view().busy && provider.view().conversation == targetConversation, "History selection stops and saves an active turn before switching");
            historyButton->triggerClick(); pumpMessages(250);
            auto* newButton = findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "New conversation"; });
            waitForCancel = true; provider.send("Running request before new conversation", "gpt-6.1-sol"); pumpMessages(250);
            check(newButton && newButton->isEnabled(), "New conversation is available during active work");
            click(*newButton, newButton->getLocalBounds().getCentre()); waitForCancel = false;
            check(!provider.view().busy && provider.view().conversation != targetConversation && provider.view().transcript.isEmpty(), "New conversation mouse action stops old work and opens an empty chat");
            check(controller.prompt->stringValue().isEmpty(), "New conversation clears the previous draft");
            historyButton->triggerClick(); pumpMessages(250);
            click(*historyButton, historyButton->getLocalBounds().getCentre());
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == controller.models; })->isVisible(), "Back arrow returns from history even with an empty conversation");
            provider.selectConversation(previousConversation); pumpMessages(250);
            panel.setSize(320, 700); panel.resized();
            checkSelectorRow();
            preview = preview.getSiblingFile("mcp-assistant-panel-narrow.png"); snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
        }
        auto deleted = provider.view().conversation; provider.clearHistory();
        bool retainedDeleted = false; for (auto item : *provider.view().conversations.getArray()) retainedDeleted |= item["id"].toString() == deleted;
        check(!retainedDeleted && provider.view().conversations.size() == 1, "Deleting one conversation retains others");
        waitForCancel = true; provider.send("Cancel pending inference", "gpt-6.1-sol"); pumpMessages(100); provider.cancel(); waitForCancel = false;
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
