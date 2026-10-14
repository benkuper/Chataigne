// Explicit, manual subscription-backed test. Runs only in a disposable engine;
// the saved account and its remembered grant are used without upgrading access.
#pragma once
namespace mcp_test
{
#if ORGANICUI_USE_MCP && ORGANICUI_USE_ASSISTANT
static int assistantWorkflowSmoke()
{
    using namespace organic::assistant;
    juce::ScopedJuceInitialiser_GUI runtime;
    McpTestApplication app; app.engine.reset(new ChataigneEngine());
    auto& mcp = *static_cast<ChataigneEngine*>(app.engine.get())->mcp;
    auto& provider = *mcp.assistant;
    auto wait = [&](int milliseconds)
    {
        auto deadline = juce::Time::getMillisecondCounterHiRes() + milliseconds;
        String previous;
        while (provider.view().busy && juce::Time::getMillisecondCounterHiRes() < deadline)
        {
            pumpMessages(50);
            // The test must never observe desktop key input or affect an open project.
            for (auto* module : ModuleManager::getInstance()->items)
                if (auto* keyboard = dynamic_cast<KeyboardModule*>(module)) KeyboardHooker::getInstance()->removeListener(keyboard);
            auto state = provider.view(); auto activity = state.workTitle;
            if (activity != previous) { std::cout << activity << '\n' << std::flush; previous = activity; }
        }
        if (provider.view().busy) { provider.cancel(); throw std::runtime_error("Assistant workflow deadline exceeded"); }
    };
    String original, smoke;
    bool restoreOriginal = false;
    auto output = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries")
        .getNonexistentChildFile("assistant-workflow-smoke", ".txt", false);
    int result = 1;
    try
    {
        wait(240000); auto state = provider.view();
        check(state.signedIn && state.models.size() > 0, "Saved subscription and models are available");
        bool modelAvailable = false;
        for (auto model : *state.models.getArray()) modelAvailable |= model["slug"].toString() == "gpt-6.1-sol";
        check(modelAvailable, "The requested GPT-6.1-Sol model is available");
        original = state.conversation;
        for (auto row : *state.conversations.getArray()) restoreOriginal |= row["id"].toString() == original;
        provider.newConversation(); smoke = provider.view().conversation;
        std::cout << "Running the original workflow prompt in a new, empty test engine\n" << std::flush;
        provider.send("create custom variables of various types and range make presets, create a sequence with 20 triggers that go to those presets. add a keyboard module that launches the sequence when doing ctrl+p and stops when doing ctrl+m for 1s", "gpt-6.1-sol");
        wait(600000); state = provider.view();
        Panel exportPanel("Assistant workflow test", *mcp.assistantController);
        exportPanel.setSize(660, 900); pumpMessages(250);
        output.replaceWithText(exportPanel.conversationText());
        Context context; context.grant = provider.grant(); context.deadline = juce::Time::getMillisecondCounterHiRes() + 30000;
        mcp.registry.call("chataigne_save", object({{"path", output.withFileExtension(".noisette").getFullPathName()}}), context);
        std::cout << "Transcript: " << output.getFullPathName() << '\n' << std::flush;
        check(state.activity == organic::assistant::Activity::completed && !state.canResume, "One prompt finishes without user recovery");
        check(state.tasks.size() >= 3, "Workflow outcomes are tracked");
        for (auto task : *state.tasks.getArray()) check(task["status"].toString() == "completed", "Every requested outcome is verified complete");

        juce::StringArray types; int presets = 0, rangedNumbers = 0;
        for (auto* group : CVGroupManager::getInstance()->items)
        {
            for (auto* item : group->values.items)
                if (auto* parameter = dynamic_cast<Parameter*>(item->controllable))
                {
                    types.addIfNotAlreadyThere(parameter->getTypeString());
                    if (dynamic_cast<FloatParameter*>(parameter) || dynamic_cast<IntParameter*>(parameter))
                    {
                        auto low = (double) parameter->minimumValue, high = (double) parameter->maximumValue;
                        check(std::isfinite(low) && std::isfinite(high) && low < high, "Numeric variables have valid ranges"); ++rangedNumbers;
                    }
                }
            for (auto* preset : group->pm->items)
            {
                auto values = preset->getValuesAsJSON();
                check(values.isObject() && values.getDynamicObject()->getProperties().size() >= group->values.items.size(), "Presets capture the group's variable values"); ++presets;
            }
        }
        check(types.size() >= 4 && rangedNumbers >= 2 && presets >= 2, "Various variable types, numeric ranges, and multiple populated presets exist");
        check(ChataigneSequenceManager::getInstance()->items.size() == 1, "Exactly one requested sequence exists");
        auto* sequence = ChataigneSequenceManager::getInstance()->items[0];
        juce::Array<double> times; int presetBindings = 0;
        for (auto* layer : sequence->layerManager->items)
            if (auto* triggers = dynamic_cast<TriggerLayer*>(layer))
                for (auto* trigger : triggers->ttm->items)
                {
                    auto time = trigger->time->doubleValue();
                    check(time >= 0 && time <= sequence->totalTime->doubleValue() && !times.contains(time), "Trigger times are distinct and within the sequence"); times.add(time);
                    auto* authored = dynamic_cast<ChataigneTimeTrigger*>(trigger); bool bound = false;
                    if (authored) for (auto* item : authored->csm->items)
                        if (auto* handler = dynamic_cast<BaseCommandHandler*>(item))
                        if (auto* command = dynamic_cast<CVCommand*>(handler->command.get()))
                            if (command->type == CVCommand::GO_TO_PRESET || command->type == CVCommand::SET_PRESET)
                                bound |= command->targetPreset && dynamic_cast<CVPreset*>(command->targetPreset->getTargetContainer());
                    check(bound, "Every sequence trigger resolves a real preset command"); ++presetBindings;
                }
        check(times.size() == 20 && presetBindings == 20, "Exactly twenty distributed preset triggers exist");
        KeyboardModule* keyboard = nullptr;
        for (auto* module : ModuleManager::getInstance()->items) if (auto* candidate = dynamic_cast<KeyboardModule*>(module)) keyboard = candidate;
        check(keyboard != nullptr, "A real keyboard module exists");
        int plays = 0, stops = 0;
        for (auto* state : StateManager::getInstance()->items)
            for (auto* processor : state->pm->items)
                if (auto* action = dynamic_cast<Action*>(processor))
                    for (auto* item : action->csmOn->items)
                        if (auto* handler = dynamic_cast<BaseCommandHandler*>(item))
                        if (auto* command = dynamic_cast<SequenceCommand*>(handler->command.get()))
                            if (command->target && command->target->getTargetContainer() == sequence)
                            {
                                plays += command->actionType == SequenceCommand::PLAY_SEQUENCE;
                                stops += command->actionType == SequenceCommand::STOP_SEQUENCE;
                            }
        check(plays > 0 && stops > 0, "Keyboard actions resolve play and stop commands to the sequence");
        sequence->stopTrigger->trigger(); pumpMessages(100);
        keyboard->keyChanged('P', true); pumpMessages(50); check(!sequence->isPlaying->boolValue(), "P alone does not start playback");
        keyboard->keyChanged('P', false); keyboard->keyChanged(VK_LCONTROL, true); pumpMessages(50);
        check(!sequence->isPlaying->boolValue(), "Ctrl alone does not start playback");
        keyboard->keyChanged('P', true); pumpMessages(100); check(sequence->isPlaying->boolValue(), "Ctrl P starts the authored sequence");
        keyboard->keyChanged('P', false); keyboard->keyChanged('M', true); pumpMessages(300); keyboard->keyChanged('M', false);
        pumpMessages(800); check(sequence->isPlaying->boolValue(), "A short Ctrl M press cancels the hold timer");
        keyboard->keyChanged('M', true); pumpMessages(550); keyboard->keyChanged(VK_LCONTROL, false); pumpMessages(650);
        check(sequence->isPlaying->boolValue(), "Releasing Ctrl cancels the continuous hold");
        keyboard->keyChanged(VK_RCONTROL, true); pumpMessages(550); check(sequence->isPlaying->boolValue(), "Right Ctrl M does not stop early");
        pumpMessages(650); check(!sequence->isPlaying->boolValue() && sequence->currentTime->doubleValue() == 0, "A full one-second Ctrl M hold stops and resets playback");
        keyboard->keyChanged('M', false); keyboard->keyChanged(VK_RCONTROL, false);
        std::cout << "Passed real one-prompt workflow and native project/playback verification\n"; result = 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Workflow smoke failed: " << error.what() << '\n';
        if (!smoke.isEmpty())
        {
            Panel exportPanel("Assistant workflow test", *mcp.assistantController); exportPanel.setSize(660, 900); pumpMessages(250);
            output.replaceWithText(exportPanel.conversationText());
            std::cerr << "Transcript: " << output.getFullPathName() << '\n';
        }
    }
    try
    {
        if (!smoke.isEmpty()) provider.clearHistory();
        if (restoreOriginal) provider.selectConversation(original);
    }
    catch (const std::exception& error) { std::cerr << "Conversation restoration failed: " << error.what() << '\n'; result = 1; }
    return result;
}
#endif
}
