#include "MainIncludes.h"
#include "MCP/ChataigneMcp.h"
#include "Module/ModuleIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "Common/Command/BaseCommandHandler.h"
#include <iostream>
#include <stdexcept>
#undef main
namespace mcp_test
{
using namespace organic::mcp;

class McpTestApplication : public OrganicApplication
{
public:
    McpTestApplication() : OrganicApplication("MCP authoring tests", false) {}
    void initialiseInternal(const String&) override {}
};
static int checks = 0;
static void check(bool condition, const char* message) { ++checks; if (!condition) throw std::runtime_error(message); }
static void rejects(const std::function<void()>& action, int expected)
{
    try { action(); } catch (const Error& e) { check(e.code == expected, "unexpected rejection code"); return; }
    throw std::runtime_error("operation was not rejected");
}
template<class T, class Match>
static T* findComponent(juce::Component& root, Match match)
{
    if (auto* typed = dynamic_cast<T*>(&root)) if (match(*typed)) return typed;
    for (auto* child : root.getChildren()) if (auto* found = findComponent<T>(*child, match)) return found;
    return nullptr;
}
static void pumpMessages(int milliseconds)
{
    auto until = juce::Time::getMillisecondCounterHiRes() + milliseconds;
    do { MSG message; while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&message); DispatchMessage(&message); } Sleep(1); } while (juce::Time::getMillisecondCounterHiRes() < until);
}
}
#include "assistant_integration_test.h"
namespace mcp_test
{
int run()
{
    juce::ScopedJuceInitialiser_GUI runtime;
    McpTestApplication app; app.engine.reset(new ChataigneEngine());
    auto directory = juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("mcp-authoring", {}, false);
    directory.createDirectory();
    int result = 0;
    try
    {
        auto* engine = static_cast<ChataigneEngine*>(app.engine.get()); auto& mcp = *engine->mcp;
        Context context; context.grant.access = Access::full; context.grant.id = "test";
        auto call = [&](const String& name, const var& args) { context.deadline = juce::Time::getMillisecondCounterHiRes() + 30000; return mcp.registry.call(name, args, context); };
        auto revisionArgs = [&](ControllableContainer* cc) { auto info = mcp.organic.describe(mcp.organic.handle(cc)); return object({{"handle", info["handle"]}, {"revision", info["revision"]}}); };
        auto create = [&](ControllableContainer* manager, const String& type, const String& name)
        {
            auto args = revisionArgs(manager); args.getDynamicObject()->setProperty("type", type); args.getDynamicObject()->setProperty("name", name);
            return call("organic_create", args);
        };
        check(!mcp.enabled->boolValue() && !mcp.server->isRunning(), "server must be opt-in");
        auto roots = call("chataigne_project", object()); check(roots["modules"].isString(), "project discovery");
        assistantChecks(directory);
        {
            Permissions uiPermissions(directory.getChildFile("panel-clients.json"));
            Server uiServer(mcp.registry, uiPermissions, "Connection panel test", "1");
            struct Host : ControllableContainer
            {
                Server& server; BoolParameter* enabled; IntParameter* port;
                Host(Server& s) : ControllableContainer("Test MCP settings"), server(s)
                { enabled = addBoolParameter("Enabled", "", false); port = addIntParameter("Port", "", 42009, 1, 65535); }
                void onContainerParameterChanged(Parameter* value) override
                { if (value == enabled) { if (enabled->boolValue()) check(server.start(port->intValue()).wasOk(), "panel listener start"); else server.stop(); } }
            } host(uiServer);
            ConnectionSettings connectionSettings(uiPermissions, uiServer, [&] { return host.port->intValue(); });
            ConnectionPanel panel("MCP Connections", connectionSettings, *host.enabled, *host.port);
            panel.setSize(720, 760);
            check(findComponent<ControllableUI>(panel, [&](const auto& ui) { return ui.controllable == connectionSettings.create; }) != nullptr, "connection actions use OrganicUI trigger controls");
            connectionSettings.clientName->setValue("Regression client"); connectionSettings.profile->setValueWithData(3);
            connectionSettings.create->trigger();
            check(uiPermissions.list().size() == 1 && uiPermissions.list()[0].access == Access::full, "panel creates remembered Full Access client");
            host.enabled->setValue(true); connectionSettings.test->trigger();
            for (int attempt = 0; attempt < 100 && !connectionSettings.result->stringValue().contains("succeeded"); ++attempt) pumpMessages(50);
            check(connectionSettings.result->stringValue().contains("succeeded"), "panel tests authenticated HTTP connection");
            auto snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            auto preview = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries/mcp-connection-panel.png");
            if (auto output = preview.createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
            connectionSettings.rotate->trigger();
            check(uiPermissions.list().size() == 1, "panel rotates existing client");
            connectionSettings.revoke->trigger(); check(uiPermissions.list().isEmpty(), "panel revokes client");
            uiServer.stop();
        }
        auto module = create(ModuleManager::getInstance(), "OSC", "MCP OSC");
        check(module["type"].toString() == "OSC", "built-in module authoring");
        auto setParameter = [&](Parameter* parameter, const var& value)
        { auto info = mcp.organic.describe(mcp.organic.handle(parameter)); return call("organic_set_value", object({{"handle", info["handle"]}, {"revision", info["revision"]}, {"value", value}})); };
        auto* oscModule = dynamic_cast<Module*>(mcp.organic.container(module["handle"].toString()));
        auto scriptData = create(oscModule->scriptManager.get(), "", "MCP Script");
        auto* script = dynamic_cast<Script*>(mcp.organic.container(scriptData["handle"].toString()));
        auto scriptPath = directory.getChildFile("probe.js"); setParameter(script->filePath, scriptPath.getFullPathName());
        call("chataigne_script_write", object({{"handle", scriptData["handle"]}, {"source", "function mcpProbe() { return 42; }"}}));
        check(call("chataigne_script_inspect", object({{"handle", scriptData["handle"]}}))["source"].toString().contains("mcpProbe"), "script source authoring and inspection");
        call("chataigne_script_reload", object({{"handle", scriptData["handle"]}}));
        check((int) call("chataigne_script_invoke", object({{"handle", scriptData["handle"]}, {"function", "mcpProbe"}, {"arguments", array({})}}))["result"] == 42, "script invocation");
        call("chataigne_script_write", object({{"handle", scriptData["handle"]}, {"source", "function broken( {"}}));
        check(script->state == Script::SCRIPT_ERROR, "broken script diagnostic evidence");
        auto scriptDiagnostics = call("chataigne_diagnostics", object()); check(juce::JSON::toString(scriptDiagnostics).contains("script_error"), "diagnostics report script failure");
        call("organic_remove", revisionArgs(script));
        auto state = create(StateManager::getInstance(), "", "MCP State");
        check(state["handle"].isString(), "state authoring");
        auto* authoredState = dynamic_cast<State*>(mcp.organic.container(state["handle"].toString()));
        auto createFirst = [&](ControllableContainer* manager, const String& name)
        {
            auto types = call("organic_types", object({{"handle", mcp.organic.handle(manager)}}))["types"];
            check(types.size() > 0, "family factory discovery"); return create(manager, types[0]["type"].toString(), name);
        };
        auto actionData = create(authoredState->pm.get(), "Action", "MCP Action");
        auto* action = dynamic_cast<Action*>(mcp.organic.container(actionData["handle"].toString()));
        createFirst(action->cdm.get(), "MCP Condition"); createFirst(action->csmOn.get(), "MCP Consequence");
        auto mappingData = create(authoredState->pm.get(), "Mapping", "MCP Mapping");
        auto* mapping = dynamic_cast<Mapping*>(mcp.organic.container(mappingData["handle"].toString()));
        createFirst(&mapping->im, "MCP Input"); create(&mapping->fm, "Remap", "MCP Remap"); createFirst(&mapping->om, "MCP Output");
        check(mapping->im.items.size() == 1 && mapping->fm.items.size() == 1 && mapping->om.items.size() == 1, "mapping processing chain authoring");
        auto multiplexData = create(authoredState->pm.get(), "Multiplex", "MCP Multiplex");
        auto* multiplex = dynamic_cast<Multiplex*>(mcp.organic.container(multiplexData["handle"].toString()));
        create(&multiplex->processorManager, "Mapping", "MCP Nested Mapping"); check(multiplex->processorManager.items.size() == 1, "multiplex authoring");
        auto destinationStateData = create(StateManager::getInstance(), "", "MCP Destination State");
        auto sourceInfo = mcp.organic.describe(state["handle"].toString()), destinationStateInfo = mcp.organic.describe(destinationStateData["handle"].toString());
        auto transition = call("chataigne_add_transition", object({{"source", sourceInfo["handle"]}, {"destination", destinationStateInfo["handle"]}, {"sourceRevision", sourceInfo["revision"]}, {"destinationRevision", destinationStateInfo["revision"]}}));
        check(StateManager::getInstance()->stm.items.size() == 1, "transition authoring");
        call("organic_undo", object({{"transaction", transition["transaction"]}})); check(StateManager::getInstance()->stm.items.isEmpty(), "transition undo");
        call("organic_remove", revisionArgs(mcp.organic.container(destinationStateData["handle"].toString())));
        createFirst(ModuleRouterManager::getInstance(), "MCP Router");
        auto dashboardData = createFirst(DashboardManager::getInstance(), "MCP Dashboard");
        auto* dashboard = dynamic_cast<Dashboard*>(mcp.organic.container(dashboardData["handle"].toString()));
        create(&dashboard->itemManager, "DashboardCommentItem", "MCP Dashboard Comment"); check(dashboard->itemManager.items.size() == 1, "dashboard authoring");
        auto parrotData = createFirst(ParrotManager::getInstance(), "MCP Parrot");
        auto* parrot = dynamic_cast<Parrot*>(mcp.organic.container(parrotData["handle"].toString()));
        createFirst(&parrot->recordManager, "MCP Record"); check(parrot->recordManager.items.size() == 1, "Parrot record authoring");
        auto group = create(CVGroupManager::getInstance(), "", "MCP Variables");
        auto* cvGroup = dynamic_cast<CVGroup*>(mcp.organic.container(group["handle"].toString())); check(cvGroup != nullptr, "custom variable group authoring");
        auto variable = create(&cvGroup->values, cvGroup->values.getTypeForControllableType("Float"), "Level");
        auto* variableItem = mcp.organic.container(variable["handle"].toString());
        Parameter* value = nullptr; for (auto* c : variableItem->controllables) if (auto* p = dynamic_cast<FloatParameter*>(c)) if (p != dynamic_cast<BaseItem*>(variableItem)->listUISize) { value = p; break; }
        check(value != nullptr, "custom variable parameter discovery");
        createFirst(cvGroup->pm.get(), "MCP Preset"); check(cvGroup->pm->items.size() == 1, "preset authoring");
        auto watch = call("chataigne_detective_watch", object({{"handle", mcp.organic.handle(value)}}));
        auto observed = call("chataigne_detective_inspect", object({{"handle", watch["handle"]}})); check((bool) observed["readOnly"], "Detective observation");
        auto valueInfo = mcp.organic.describe(mcp.organic.handle(value));
        auto set = object({{"handle", valueInfo["handle"]}, {"revision", valueInfo["revision"]}, {"value", 0.5}});
        context.grant.access = Access::inspect;
        rejects([&] { call("organic_set_value", set); }, -32003);
        context.grant.access = Access::control;
        auto edited = call("organic_set_value", set); check(value->doubleValue() == 0.5, "typed parameter edit");
        context.grant.access = Access::full;
        rejects([&] { call("organic_set_value", set); }, -32006);
        call("organic_undo", object({{"transaction", edited["transaction"]}})); check(value->doubleValue() != 0.5, "named undo");
        call("organic_redo", object({{"transaction", edited["transaction"]}})); check(value->doubleValue() == 0.5, "named redo");
        valueInfo = mcp.organic.describe(mcp.organic.handle(value)); set.getDynamicObject()->setProperty("revision", valueInfo["revision"]);
        auto preview = call("organic_batch", object({{"preview", true}, {"operations", array({object({{"name", "organic_set_value"}, {"arguments", set}})})}}));
        check((bool) preview["valid"], "batch preview");
        set.getDynamicObject()->setProperty("value", "invalid");
        rejects([&] { call("organic_set_value", set); }, -32602);
        check(value->doubleValue() == 0.5, "invalid edit did not mutate");
        auto duplicate = call("organic_duplicate", revisionArgs(variableItem));
        check(cvGroup->values.items.size() == 2, "duplicate custom variable");
        auto secondGroup = create(CVGroupManager::getInstance(), "", "MCP Destination");
        auto* destination = dynamic_cast<CVGroup*>(mcp.organic.container(secondGroup["handle"].toString()));
        auto move = revisionArgs(mcp.organic.container(duplicate["handle"].toString()));
        auto destinationInfo = mcp.organic.describe(mcp.organic.handle(&destination->values));
        move.getDynamicObject()->setProperty("destination", destinationInfo["handle"]);
        move.getDynamicObject()->setProperty("destinationRevision", destinationInfo["revision"]);
        auto moved = call("organic_move", move);
        check(destination->values.items.size() == 1 && cvGroup->values.items.size() == 1, "move between compatible managers");
        check(mcp.organic.container(moved["handle"].toString())->parentContainer == &destination->values, "move returns replacement handle");
        call("organic_undo", object({{"transaction", moved["transaction"]}}));
        check(destination->values.items.isEmpty() && cvGroup->values.items.size() == 2, "move undo");
        call("organic_remove", revisionArgs(destination));
        auto* helper = new ControllableContainer("MCP Test Helpers"); engine->addChildControllableContainer(helper, true);
        auto* secondValue = helper->addFloatParameter("Second", "", 0, 0, 1);
        auto firstInfo = mcp.organic.describe(mcp.organic.handle(value)), secondInfo = mcp.organic.describe(mcp.organic.handle(secondValue));
        auto batch = call("organic_batch", object({{"operations", array({
            object({{"name", "organic_set_value"}, {"arguments", object({{"handle", firstInfo["handle"]}, {"revision", firstInfo["revision"]}, {"value", 0.75}})}}),
            object({{"name", "organic_set_value"}, {"arguments", object({{"handle", secondInfo["handle"]}, {"revision", secondInfo["revision"]}, {"value", 0.25}})}})})}}));
        check((bool) batch["success"] && value->doubleValue() == 0.75 && secondValue->doubleValue() == 0.25, "batch authoring");
        call("organic_undo", object({{"transaction", batch["transaction"]}}));
        check(value->doubleValue() == 0.5 && secondValue->doubleValue() == 0, "batch groups undo");
        rejects([&] { call("organic_undo", object({{"transaction", edited["transaction"]}})); }, -32006);
        auto* file = helper->addFileParameter("Asset", "", directory.getSiblingFile("outside.wav").getFullPathName());
        auto fileInfo = mcp.organic.describe(mcp.organic.handle(file));
        context.grant.access = Access::edit; context.grant.folders = {directory.getFullPathName()};
        auto fileEdit = call("organic_set_value", object({{"handle", fileInfo["handle"]}, {"revision", fileInfo["revision"]}, {"value", directory.getChildFile("inside.wav").getFullPathName()}}));
        rejects([&] { call("organic_undo", object({{"transaction", fileEdit["transaction"]}})); }, -32003);
        context.grant.access = Access::full;
        call("organic_undo", object({{"transaction", fileEdit["transaction"]}}));
        engine->removeChildControllableContainer(helper);
        auto seq = create(ChataigneSequenceManager::getInstance(), "", "MCP Sequence");
        auto* sequence = dynamic_cast<Sequence*>(mcp.organic.container(seq["handle"].toString())); check(sequence != nullptr, "sequence authoring");
        auto layerData = create(sequence->layerManager.get(), "Audio", "MCP Audio Layer");
        auto* audioLayer = dynamic_cast<AudioLayer*>(mcp.organic.container(layerData["handle"].toString()));
        createFirst(&audioLayer->clipManager, "MCP Audio Clip"); check(audioLayer->clipManager.items.size() == 1, "timeline clip authoring");
        create(sequence->layerManager.get(), "Trigger", "MCP Trigger Layer"); check(sequence->layerManager->items.size() == 2, "timeline layer authoring");
        call("timeline_transport", object({{"handle", seq["handle"]}, {"action", "play"}})); pumpMessages(30);
        check(sequence->isPlaying->boolValue(), "live playback");
        call("timeline_transport", object({{"handle", seq["handle"]}, {"action", "pause"}})); check(!sequence->isPlaying->boolValue(), "pause playback");
        auto cue = revisionArgs(sequence); cue.getDynamicObject()->setProperty("seconds", 0.5);
        auto addedCue = call("timeline_add_cue", cue); check(sequence->cueManager->items.size() == 1, "cue authoring");
        call("organic_undo", object({{"transaction", addedCue["transaction"]}})); check(sequence->cueManager->items.isEmpty(), "cue undo");
        auto seekResult = call("timeline_transport", object({{"handle", seq["handle"]}, {"action", "seek"}, {"seconds", 0.25}}));
        auto expectedTime = sequence->getTimeForFrame(sequence->getFrameForTime(0.25));
        check(std::abs(sequence->currentTime->doubleValue() - expectedTime) < 1.0e-6
            && (double) seekResult["seconds"] == sequence->currentTime->doubleValue(), "timeline seek respects frame resolution");
        rejects([&] { call("timeline_transport", object({{"handle", seq["handle"]}, {"action", "seek"}, {"seconds", -1}})); }, -32602);
        auto* automation = new Automation("MCP Automation"); engine->addChildControllableContainer(automation, true);
        auto key = revisionArgs(automation); key.getDynamicObject()->setProperty("position", 0.5); key.getDynamicObject()->setProperty("value", 0.5);
        call("timeline_add_key", key); check(automation->items.size() == 1, "automation key authoring");
        engine->removeChildControllableContainer(automation);
        auto removedHandle = module["handle"].toString();
        auto removed = call("organic_remove", revisionArgs(mcp.organic.container(removedHandle)));
        rejects([&] { mcp.organic.container(removedHandle); }, -32005);
        call("organic_undo", object({{"transaction", removed["transaction"]}})); check(ModuleManager::getInstance()->items.size() == 1, "manager removal undo");
        auto diagnostics = call("chataigne_diagnostics", object()); check((bool) diagnostics["readOnly"], "diagnostics are read-only");
        auto project = directory.getChildFile("roundtrip.noisette");
        call("chataigne_save", object({{"path", project.getFullPathName()}})); check(project.existsAsFile(), "project save");
        auto oldGeneration = mcp.organic.generation(); auto oldRoot = roots["root"].toString();
        call("chataigne_new", object({{"discardChanges", true}}));
        check(mcp.organic.generation() > oldGeneration, "project replacement generation");
        rejects([&] { mcp.organic.container(oldRoot); }, -32005);
        call("chataigne_open", object({{"path", project.getFullPathName()}, {"discardChanges", true}}));
        check(ModuleManager::getInstance()->items.size() == 1, "module save/reload");
        check(CVGroupManager::getInstance()->items.size() == 1, "custom variables save/reload");
        check(StateManager::getInstance()->items.size() == 1, "state save/reload");
        check(ChataigneSequenceManager::getInstance()->items.size() == 1, "sequence save/reload");
        std::cout << "Passed " << checks << " MCP authoring checks\n";
    }
    catch (const std::exception& error) { std::cerr << "MCP authoring failed: " << error.what() << '\n'; result = 1; }
    app.engine.reset(); directory.deleteRecursively(); return result;
}
}
int main(int argc, char** argv)
{
#if ORGANICUI_USE_ASSISTANT
    if (argc == 2 && (juce::String(argv[1]) == "--subscription-smoke" || juce::String(argv[1]) == "--saved-subscription-smoke"))
    {
        juce::ScopedJuceInitialiser_GUI runtime;
        mcp_test::McpTestApplication app; app.engine.reset(new ChataigneEngine());
        auto* engine = static_cast<ChataigneEngine*>(app.engine.get());
        auto* provider = engine->mcp->assistant.get();
        if (!provider) { std::cerr << "Assistant provider unavailable\n"; return 1; }
        try
        {
            if (juce::String(argv[1]) == "--saved-subscription-smoke") provider->refreshModels(); else provider->signIn(false);
            auto deadline = juce::Time::getMillisecondCounterHiRes() + 240000;
            while (provider->view().busy && juce::Time::getMillisecondCounterHiRes() < deadline) mcp_test::pumpMessages(50);
            if (provider->view().busy) provider->cancel();
            auto state = provider->view();
            if (state.models.size() == 0) { std::cerr << state.status << '\n'; return 1; }
            // The smoke conversation uses the user's remembered grant. Its explicit
            // request is read-only and the test does not upgrade access.
            auto previousLength = state.transcript.length();
            provider->send("Call the chataigne_project tool to inspect this empty test project. Do not edit or run output. Then say: Chataigne assistant connected.", state.models[0]["slug"].toString());
            deadline = juce::Time::getMillisecondCounterHiRes() + 240000;
            while (provider->view().busy && juce::Time::getMillisecondCounterHiRes() < deadline) mcp_test::pumpMessages(50);
            if (provider->view().busy) provider->cancel();
            state = provider->view();
            std::cout << state.status << '\n';
            auto passed = state.status == "Response complete." && state.transcript.substring(previousLength).contains("[Tool: chataigne_project]");
            std::cout << (passed ? "Passed real subscription-backed assistant smoke test\n" : "Subscription smoke did not complete a local tool call\n");
            return passed ? 0 : 1;
        }
        catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
    }
#endif
    return mcp_test::run();
}
