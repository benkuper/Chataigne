#include "../../Modules/juce_organicui/assistant/AssistantProvider.h"
#include "../../Modules/juce_organicui/assistant/ClaudeProvider.h"
#include <iostream>

int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI runtime;
    using namespace organic::mcp;
    auto appDirectory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("Chataigne");
    bool claudeMode = argc == 2 && juce::String(argv[1]).startsWith("--claude");
    bool claudeCheck = argc == 2 && juce::String(argv[1]) == "--claude-check";
    if (claudeMode) appDirectory = juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("claude-assistant-smoke", {}, false);
    struct Cleanup { juce::File directory; bool temporary; ~Cleanup() { if (temporary) directory.deleteRecursively(); } } cleanup {appDirectory, claudeMode};
    try
    {
        Registry registry; Operation inspectProject;
        inspectProject.name = "chataigne_project"; inspectProject.description = "Read the empty native assistant smoke-test project.";
        inspectProject.output = openObjectSchema(); inspectProject.execute = [](const var&, Context&) { return object({{"name", "Assistant smoke test"}, {"modules", 0}, {"readOnly", true}}); }; registry.add(inspectProject);
        Permissions permissions(appDirectory.getChildFile("MCP/clients.json"));
        auto generation = [] { return (juce::int64) 1; }; auto context = [] { return object({{"name", "Empty test project"}}); };
        std::unique_ptr<organic::assistant::Provider> selected;
        if (claudeMode) selected = std::make_unique<organic::assistant::ClaudeProvider>(registry, permissions, appDirectory.getChildFile("Assistant"), "Chataigne", generation, context);
        else selected = std::make_unique<organic::assistant::ChatGptProvider>(registry, permissions, appDirectory.getChildFile("Assistant"), "Chataigne", generation, context);
        auto& provider = *selected;
        bool savedSession = argc == 2 && juce::String(argv[1]) == "--saved-session";
        if (claudeCheck) provider.refreshModels();
        else if (!savedSession) { provider.cancel(); provider.signIn(false); }
        auto wait = [&]
        {
            auto end = juce::Time::getMillisecondCounterHiRes() + 240000;
            while (provider.view().busy && juce::Time::getMillisecondCounterHiRes() < end) juce::MessageManager::getInstance()->runDispatchLoopUntil(25);
            if (provider.view().busy) provider.cancel();
        };
        wait(); auto state = provider.view();
        if (claudeCheck)
        {
            bool passed = state.signedIn || state.status.contains("claude auth login");
            std::cout << (passed ? "Passed native Claude Code process and sign-in detection check\n" : "Claude Code process check failed\n") << state.status << '\n';
            return passed ? 0 : 1;
        }
        if (state.models.size() == 0) { std::cerr << state.status << '\n'; return 1; }
        provider.newConversation();
        provider.send("Call the chataigne.chataigne_project function now. You must obtain its result before answering. Then state the returned project name. Make no edits.", state.models[0]["slug"].toString());
        wait(); state = provider.view(); std::cout << state.status << '\n';
        bool passed = state.activity == organic::assistant::Activity::completed && state.transcript.contains("[Tool: chataigne_project]");
        if (!passed) std::cout << state.transcript.substring(juce::jmax(0, state.transcript.length() - 3000)).toStdString() << '\n';
        std::cout << (passed ? "Passed subscription-backed native assistant and local tool smoke test\n" : "No completed local tool call\n"); return passed ? 0 : 1;
    }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
