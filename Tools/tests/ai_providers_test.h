// Native provider integration, shared panel switching and settings migration.
#pragma once
namespace mcp_test
{
static void aiProviderChecks(const juce::File& directory)
{
#if ORGANICUI_USE_ASSISTANT
    using namespace organic::assistant;
    using organic::assistant::Activity;
    Registry registry; Permissions permissions(directory.getChildFile("claude-clients.json")); int reads = 0, edits = 0, inferences = 0, logins = 0;
    bool authoring = false, hold = false;
    Operation read; read.name = "organic_fixture_read"; read.output = openObjectSchema();
    read.execute = [&](const var&, Context&) { ++reads; return object({{"configured", edits}}); }; registry.add(read);
    Operation edit; edit.name = "organic_fixture_edit"; edit.capabilities = organic::mcp::edit;
    edit.execute = [&](const var&, Context&) { ++edits; return object(); }; registry.add(edit);
    ClaudeProvider::Runner runner = [&](const juce::StringArray& args, const String& input, std::shared_ptr<std::atomic<bool>> cancelled) -> ClaudeProvider::ProcessResult
    {
        if (args[1] == "auth")
        {
            if (args[2] == "login") ++logins;
            return {juce::JSON::toString(object({{"loggedIn", true}}), true), 0};
        }
        if (hold) { while (!cancelled->load()) juce::Thread::sleep(5); throw Error(-32800, "Stopped Claude fixture"); }
        check(args.contains("--strict-mcp-config") && args.contains("--setting-sources") && args[args.indexOf("--tools") + 1].isEmpty(), "Embedded Claude disables built-in tools and unrelated MCP/config discovery");
        check(!args.contains("--bare") && !args.contains("--dangerously-skip-permissions"), "Claude retains its native sign-in without bypassing tool permissions");
        check(!input.contains("mcp_token") && !input.contains("refresh_token"), "Claude inference contains no protected local credentials");
        auto context = parseBounded(input.substring(input.indexOfChar('{'))); var calls = array({}); String text;
        auto invoke = [&](const String& name, const var& args) { calls.append(object({{"name", name}, {"arguments", juce::JSON::toString(args, true)}})); };
        auto phase = inferences++;
        if (!authoring)
        {
            if (phase == 0) invoke("organic_fixture_read", object()); else text = "Claude assistant connected.";
        }
        else
        {
            auto task = [&](const String& status, const String& evidence, const String& verification)
            {
                return object({{"tasks", array({object({{"id", "configure"}, {"description", "Configure and verify the requested workflow"}, {"status", status},
                    {"evidence", evidence}, {"nextStep", status == "blocked" ? "Choose Edit in the panel, then Resume unfinished." : ""},
                    {"verifiedBy", verification.isEmpty() ? array({}) : array({verification})}})})}});
            };
            if (phase == 0) { invoke("assistant_task", task("pending", "", "")); invoke("organic_fixture_edit", object()); }
            else if (phase == 1) invoke("organic_fixture_read", object());
            else if (phase == 2)
            {
                bool denied = false; String verification;
                for (auto item : *context["conversation"].getArray())
                {
                    if (item["role"].toString() == "user") { denied = false; verification.clear(); }
                    if (item["type"].toString() == "function_call" && item["name"].toString() == "organic_fixture_read") verification = item["call_id"].toString();
                    if (item["type"].toString() == "function_call_output") denied |= (int) juce::JSON::parse(item["output"].toString())["error"] == -32003;
                }
                invoke("assistant_task", task(denied ? "blocked" : "completed", denied ? "Edit permission is required" : "Final configuration inspected", denied ? String() : verification));
            }
            else text = "Workflow checked.";
        }
        return {juce::JSON::toString(object({{"is_error", false}, {"structured_output", object({{"text", text}, {"calls", calls}})}}), true), 0};
    };
    ClaudeProvider claude(registry, permissions, directory.getChildFile("claude"), "Chataigne", [] { return (juce::int64) 1; }, [] { return object(); }, runner);
    auto wait = [&](Provider& provider)
    {
        for (int i = 0; i < 400 && provider.view().busy; ++i) pumpMessages(25);
        check(!provider.view().busy, "Provider operation completes within deadline");
    };
    check(!claude.view().signedIn && claude.info().id == "claude", "Claude waits for explicit connection and exposes provider metadata");
    claude.signIn(false); wait(claude);
    check(claude.view().signedIn && claude.view().models.size() == 4 && claude.grant().access == Access::inspect && logins == 0, "Claude connects using existing CLI sign-in with an Inspect grant");
    claude.send("Inspect this project", "default"); wait(claude);
    check(reads == 1 && edits == 0 && claude.view().activity == Activity::completed && claude.view().transcript.contains("Claude assistant connected"), "Claude inference executes local read-only tools and retains the result");
    authoring = true; inferences = 0; claude.send("Configure and verify the requested workflow", "sonnet", "high"); wait(claude);
    check(edits == 0 && claude.view().activity == Activity::failed, "Claude cannot bypass the remembered Inspect grant");
    inferences = 0; claude.setAccess(Access::full, true, {}); claude.send("Configure and verify the requested workflow", "opus"); wait(claude);
    check(edits == 1 && claude.view().activity == Activity::completed, "Claude shares verified completion and authorized local authoring with ChatGPT");
    auto conversation = claude.view().conversation, transcript = claude.view().transcript;
    {
        ClaudeProvider restored(registry, permissions, directory.getChildFile("claude"), "Chataigne", [] { return (juce::int64) 1; }, [] { return object(); }, runner); wait(restored);
        check(restored.view().signedIn && restored.view().conversation == conversation && restored.view().transcript == transcript, "Claude reconnects and restores its own protected conversation on restart");
    }
    hold = true; claude.send("Cancel this request", "default"); pumpMessages(50); claude.cancel(); hold = false;
    check(!claude.view().busy && claude.view().activity == Activity::stopped, "Stopping Claude cancels the running transport");
    auto credential = claude.grant().id; permissions.revoke(credential); claude.refreshModels(); wait(claude);
    check(claude.view().activity == Activity::failed && permissions.list().isEmpty(), "Claude refresh cannot silently recreate a revoked local grant");
    claude.signIn(false); wait(claude); check(claude.grant().access == Access::inspect, "Explicit Claude reconnection restores access at Inspect");
    claude.signOut(); check(!claude.view().signedIn && permissions.list().isEmpty() && logins == 0, "Disconnecting Claude revokes local access and preserves the CLI's own account");

    struct FixtureProvider : Provider
    {
        ProviderInfo metadata; View state; Grant permission; int cancellations = 0;
        FixtureProvider(String id, String name) : metadata{id, name, "Connect " + name, "Connect " + name, "https://example.test", false}
        { state.active = id; state.signedIn = true; state.models = array({object({{"slug", id + "-model"}, {"name", name + " model"}})}); state.conversation = id + "-conversation"; state.transcript = "\nAssistant: " + name + " conversation"; }
        ProviderInfo info() const override { return metadata; }
        juce::StringArray efforts(const String&) const override { return {"low", "high"}; }
        View view() const override { return state; }
        void signIn(bool) override {}
        void select(const String&) override {}
        void refreshModels() override {}
        void send(const String& text, const String& model, const String& effort) override
        {
            if (state.busy) state.pendingMessages.append(object({{"id", juce::Uuid().toString()}, {"text", text}, {"kind", "queue"}, {"model", model}, {"effort", effort}}));
            else { state.busy = true; state.turnActive = true; }
        }
        void steer(const String& text) override { state.pendingMessages.append(object({{"id", juce::Uuid().toString()}, {"text", text}, {"kind", "steer"}})); }
        void steerQueued(const String& id) override
        {
            for (int i = 0; i < state.pendingMessages.size(); ++i) if (state.pendingMessages[i]["id"].toString() == id)
            { auto message = state.pendingMessages[i]; message.getDynamicObject()->setProperty("kind", "steer"); state.pendingMessages.getArray()->remove(i); state.pendingMessages.append(message); break; }
        }
        void removeQueued(const String& id) override
        { for (int i = state.pendingMessages.size(); --i >= 0;) if (state.pendingMessages[i]["id"].toString() == id) state.pendingMessages.getArray()->remove(i); }
        void resume(const String&, const String&, const String& guidance) override { state.busy = state.turnActive = true; state.transcript += guidance; }
        void runQueue() override { state.queuePaused = false; }
        void cancel() override { ++cancellations; state.busy = state.turnActive = false; }
        void signOut() override {}
        void clearHistory() override { state.transcript.clear(); }
        void newConversation() override { state.transcript.clear(); }
        void selectConversation(const String&) override {}
        Grant grant() const override { return permission; }
        void setAccess(Access access, bool advanced, juce::StringArray paths) override { permission.access = access; permission.advancedExecution = advanced; permission.folders = paths; }
    } first("chatgpt", "ChatGPT"), second("claude", "Claude");
    Controller chatgpt(first), other(second); Hub hub({&chatgpt, &other});
    check(hub.settings.niceName == "AI Assistant" && chatgpt.settings.parentContainer == &hub.settings && other.settings.parentContainer == &hub.settings, "One AI Assistant settings group contains provider-specific children");
    chatgpt.prompt->setValue("ChatGPT draft"); other.prompt->setValue("Claude draft"); first.state.busy = true;
    hub.providers->setValueWithData("claude");
    check(&hub.active() == &other && !first.state.busy && first.cancellations == 1 && chatgpt.prompt->stringValue() == "ChatGPT draft", "Switching provider stops previous work and preserves independent drafts");
    {
        HubPanel panel("AI Assistant", hub); panel.setSize(660, 900); panel.setVisible(true); panel.resized(); pumpMessages(250);
        auto* selector = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == hub.providers; });
        check(selector && selector->isVisible() && selector->getHeight() > 0, "Shared panel exposes the provider selector");
        auto* activePrompt = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == other.prompt; });
        auto* inactivePrompt = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == chatgpt.prompt; });
        auto visibleInPanel = [&](const juce::Component* component)
        {
            for (auto* current = component; current; current = current->getParentComponent())
                if (!current->isVisible()) return false;
                else if (current == &panel) return true;
            return false;
        };
        check(activePrompt && visibleInPanel(activePrompt) && inactivePrompt && !visibleInPanel(inactivePrompt), "Shared panel shows only the selected provider's conversation and controls");
        auto preview = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries/ai-assistant-providers.png");
        if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        panel.setSize(320, 700); panel.resized();
        check(selector->getWidth() > 0 && activePrompt->getWidth() > 0, "Provider selector and conversation fit a narrow dock");
        if (auto output = preview.getSiblingFile("ai-assistant-providers-narrow.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        second.state.busy = second.state.turnActive = true; second.state.activity = Activity::working;
        second.state.workTitle = "Configuring custom variables..."; second.state.workDetail = "Setting value types and ranges";
        other.prompt->setValue("A queued follow-up"); other.send->trigger(); pumpMessages(250);
        auto* steer = findComponent<juce::TextButton>(panel, [&](auto& button) { return button.getTooltip().contains("Ctrl+Shift+Enter")
            && button.getParentComponent() && visibleInPanel(button.getParentComponent()); });
        auto* stop = findComponent<juce::Button>(panel, [&](const auto& button) { return button.getName() == "Stop" && visibleInPanel(&button); });
        auto* actions = findComponent<juce::Button>(panel, [&](const auto& button) { return button.getName() == "Message actions" && visibleInPanel(&button); });
        check(steer && stop && actions && actions->getBounds().getCentreY() == stop->getBounds().getCentreY(), "Combined Send/Queue/Stop and the actions menu share one row in a narrow dock");
        check(!visibleInPanel(steer) && stop->isEnabled(), "An empty message box keeps Stop enabled and hides steering");
        check(!findComponent<juce::Button>(panel, [](const auto& button) { return button.getName() == "Usage"; }), "The redundant Usage/status footer is removed");
        check(!findComponent<juce::Label>(panel, [](const auto& label) { return label.getText() == "CURRENT ACTIVITY"; }), "Activity has no redundant heading");
        auto* currentActivity = findComponent<juce::Component>(panel, [&](const auto& component) { return component.getName() == "Assistant current activity" && visibleInPanel(&component); });
        auto* activityTitle = findComponent<juce::Label>(panel, [&](const auto& label) { return label.getName() == "Assistant current activity title" && visibleInPanel(&label); });
        check(currentActivity && activityTitle && activityTitle->getText() == "Configuring custom variables...", "Current phase is visible without expanding tool calls");
        check(panel.getLocalBounds().contains(panel.getLocalArea(currentActivity->getParentComponent(), currentActivity->getBounds())) && currentActivity->getHeight() == 64, "Compact activity fits in a narrow dock alongside the queue and composer");
        auto* pending = findComponent<juce::Component>(panel, [&](const auto& component) { return component.getName() == "Pending assistant messages" && visibleInPanel(&component); });
        check(pending && visibleInPanel(pending) && second.state.pendingMessages.size() == 1 && other.prompt->stringValue().isEmpty(), "Queued follow-ups are visible and clear the accepted draft");
        other.prompt->setValue("Use a different direction"); pumpMessages(250);
        check(stop->getName() == "Queue" && steer->isEnabled() && visibleInPanel(steer) && steer->getButtonText() == "Steer", "Typing during a request shows Steer and changes the primary button from Stop to Queue");
        check(steer->getBounds().getCentreY() == stop->getBounds().getCentreY() && !steer->getBounds().intersects(stop->getBounds()), "Steer shares the action row without overlapping Queue");
        steer->triggerClick(); pumpMessages(250);
        check(second.state.pendingMessages.size() == 2 && second.state.pendingMessages[1]["kind"].toString() == "steer" && other.prompt->stringValue().isEmpty(), "Steer button submits the draft as steering");
        if (auto output = preview.getSiblingFile("ai-assistant-steering-narrow.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        other.prompt->setValue("Keep this draft"); pumpMessages(250);
        if (auto output = preview.getSiblingFile("ai-assistant-controls-narrow.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        panel.setSize(660, 900); panel.resized(); pumpMessages(250);
        check(activePrompt->getBottom() <= stop->getY() && steer->getBounds().getCentreY() == stop->getBounds().getCentreY(), "Wide composer places its actions below the prompt on one compact control row");
        auto* modelControl = findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == other.models; });
        check(modelControl && modelControl->getBounds().getCentreY() == stop->getBounds().getCentreY()
            && modelControl->getRight() < steer->getX(), "Model selection and message actions share one row when space permits");
        if (auto output = preview.getSiblingFile("ai-assistant-controls-wide.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        panel.setSize(320, 700); panel.resized(); pumpMessages(250);
        auto* steerMessage = findComponent<juce::Button>(*pending, [](const auto& button) { return button.getName() == "Steer" && button.isEnabled(); });
        check(steerMessage != nullptr, "Queued prompts offer Steer beside Edit and Remove"); steerMessage->triggerClick(); pumpMessages(250);
        check(second.state.pendingMessages.size() == 2 && second.state.pendingMessages[1]["kind"].toString() == "steer" && second.state.pendingMessages[1]["text"].toString() == "A queued follow-up" && other.prompt->stringValue() == "Keep this draft", "Steering a queued prompt makes it the latest instruction without duplicating it or clearing the composer");
        other.prompt->setValue(""); pumpMessages(250);
        auto* editMessage = findComponent<juce::Button>(*pending, [](auto& button) { return button.getName() == "Edit" && button.getTooltip().contains("A queued follow-up"); });
        check(editMessage != nullptr, "Pending messages offer editing"); editMessage->triggerClick(); pumpMessages(250);
        check(other.prompt->stringValue() == "A queued follow-up" && second.state.pendingMessages.size() == 1, "Editing a queued message moves it back to the composer");
        check(stop->getName() == "Queue", "Editing a queued prompt also updates the primary action");
        stop->triggerClick(); pumpMessages(250);
        check(other.prompt->stringValue().isEmpty() && stop->getName() == "Stop" && second.state.pendingMessages.size() == 2, "The combined primary button queues the draft and returns to Stop");
        other.prompt->setValue("Draft before stop"); pumpMessages(250);
        actions->triggerClick(); pumpMessages(100);
        auto* menu = juce::Component::getCurrentlyModalComponent(); check(menu != nullptr, "Message actions opens an accessible popup menu");
        menu->keyPressed(juce::KeyPress(juce::KeyPress::upKey)); menu->keyPressed(juce::KeyPress(juce::KeyPress::returnKey)); pumpMessages(250);
        check(!second.state.busy && other.prompt->stringValue() == "Draft before stop", "Stop is available from the menu while typing and preserves the draft");
        check(!visibleInPanel(steer), "Steer disappears when a request stops, even with a draft");
        second.state.busy = second.state.turnActive = false; second.state.queuePaused = second.state.canResume = true; pumpMessages(250);
        check(!visibleInPanel(currentActivity), "Current activity animation disappears when work stops");
        auto* resume = findComponent<juce::Button>(panel, [&](const auto& button) { return button.getName() == "Resume unfinished" && visibleInPanel(&button); });
        auto* runQueue = findComponent<juce::Button>(panel, [&](const auto& button) { return button.getName() == "Run queue" && visibleInPanel(&button); });
        check(resume && runQueue && visibleInPanel(resume) && visibleInPanel(runQueue) && !resume->getBounds().intersects(runQueue->getBounds()), "Paused work exposes distinct resume and queue actions");
        other.prompt->setValue("The device is connected now"); pumpMessages(250); resume->triggerClick(); pumpMessages(250);
        check(second.state.turnActive && other.prompt->stringValue().isEmpty() && second.state.transcript.contains("The device is connected now"), "Resume includes missing information from the composer");
        second.cancel(); other.prompt->setValue("");
        second.state.pendingMessages = array({}); second.state.canResume = false;
        second.state.transcript = "\n[Status: Attention] Some work needs your attention.\n\n"
            "Created five custom variables, three presets, and twenty sequence triggers.\n\n"
            "The keyboard module is ready; <warning>connect the device to test input</warning>. "
            "<error>The external device did not respond.</error> Your project changes are saved.\n\n"
            "What to try: Connect the device, then choose Resume unfinished.\n\n"
            "Verified complete:\n- Variables, presets, and sequence bindings.\n";
        second.state.recovery = "Connect the device, then choose Resume unfinished.";
        pumpMessages(250);
        auto* conversation = findComponent<Panel>(panel, [&](const auto& candidate) { return visibleInPanel(&candidate); });
        check(conversation && conversation->conversationText().contains("Your project changes are saved.")
            && !conversation->conversationText().contains("<error>") && !conversation->conversationText().contains("<warning>"), "Readable transcript strips severity markup and retains normal progress and recovery instructions");
        for (auto width : {320, 660})
        {
            panel.setSize(width, 900); panel.resized(); pumpMessages(250);
            auto file = preview.getSiblingFile(width == 320 ? "ai-assistant-severity-narrow.png" : "ai-assistant-severity-wide.png");
            if (auto output = file.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(panel.createComponentSnapshot(panel.getLocalBounds()), *output); }
        }
    }
    auto* global = GlobalSettings::getInstance(); auto* existing = global->getControllableContainerByName("AI Assistant", true);
    if (existing) global->removeChildControllableContainer(existing);
    global->addChildControllableContainer(&hub.settings);
    struct RestoreSettings { GlobalSettings* global; ControllableContainer *test, *previous; ~RestoreSettings() { global->removeChildControllableContainer(test); if (previous) global->addChildControllableContainer(previous); } } restore {global, &hub.settings, existing};
    auto legacy = chatgpt.settings.getJSONData(true).clone();
    chatgpt.includeProjectContext->setValue(false); legacy = chatgpt.settings.getJSONData(true).clone(); chatgpt.includeProjectContext->setValue(true);
    global->loadJSONData(object({{"containers", object({{"chatGPTAssistant", legacy}})}}));
    check(!chatgpt.includeProjectContext->boolValue() && chatgpt.settings.niceName == "ChatGPT" && other.includeProjectContext->boolValue(), "Legacy ChatGPT preferences migrate into its provider group without affecting Claude");
    hub.providers->setValueWithData("claude"); auto saved = hub.settings.getJSONData(true).clone(); hub.providers->setValueWithData("chatgpt"); hub.settings.loadJSONData(saved);
    check(&hub.active() == &other, "Provider selection restores with Global Settings");
    auto* factory = ShapeShifterFactory::getInstance(); auto* definition = new ShapeShifterDefinition("AI Assistant", [&](const String& name) -> ShapeShifterContent* { return new HubPanel(name, hub); });
    factory->defs.add(definition); factory->aliases.set("ChatGPT Assistant", "AI Assistant");
    { std::unique_ptr<ShapeShifterContent> migrated(ShapeShifterFactory::createContent("ChatGPT Assistant")); check(migrated && migrated->contentName == "AI Assistant", "Saved ChatGPT panels migrate to the single AI Assistant panel"); }
    factory->defs.removeObject(definition); factory->aliases.remove("ChatGPT Assistant");
#endif
}
}
