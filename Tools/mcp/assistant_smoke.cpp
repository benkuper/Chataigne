#include "../../Modules/juce_organicui/assistant/AssistantProvider.h"
#include <iostream>

int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI runtime;
    using namespace organic::mcp;
    auto appDirectory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("Chataigne");
    try
    {
        Registry registry; Operation inspectProject;
        inspectProject.name = "chataigne_project"; inspectProject.description = "Read the empty native assistant smoke-test project.";
        inspectProject.output = openObjectSchema(); inspectProject.execute = [](const var&, Context&) { return object({{"name", "Assistant smoke test"}, {"modules", 0}, {"readOnly", true}}); }; registry.add(inspectProject);
        Permissions permissions(appDirectory.getChildFile("MCP/clients.json"));
        organic::assistant::ChatGptProvider provider(registry, permissions, appDirectory.getChildFile("Assistant"), "Chataigne", [] { return (juce::int64) 1; }, [] { return object({{"name", "Empty test project"}}); });
        bool savedSession = argc == 2 && juce::String(argv[1]) == "--saved-session";
        if (savedSession) provider.refreshModels(); else provider.signIn(false);
        auto wait = [&]
        {
            auto end = juce::Time::getMillisecondCounterHiRes() + 240000;
            while (provider.view().busy && juce::Time::getMillisecondCounterHiRes() < end) juce::MessageManager::getInstance()->runDispatchLoopUntil(25);
            if (provider.view().busy) provider.cancel();
        };
        wait(); auto state = provider.view();
        if (state.models.size() == 0) { std::cerr << state.status << '\n'; return 1; }
        provider.clearHistory();
        provider.send("Call the chataigne.chataigne_project function now. You must obtain its result before answering. Then state the returned project name. Make no edits.", state.models[0]["slug"].toString());
        wait(); state = provider.view(); std::cout << state.status << '\n';
        bool passed = state.status == "Response complete." && state.transcript.contains("[Tool: chataigne_project]");
        if (!passed) std::cout << state.transcript.substring(juce::jmax(0, state.transcript.length() - 3000)).toStdString() << '\n';
        std::cout << (passed ? "Passed subscription-backed native assistant and local tool smoke test\n" : "No completed local tool call\n"); return passed ? 0 : 1;
    }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
