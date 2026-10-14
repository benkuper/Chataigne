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
#include "ai_providers_test.h"
#include "assistant_workflow_smoke.h"
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
        aiProviderChecks(directory);
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
            panel.setSize(320, 760); snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            if (auto output = preview.getSiblingFile("mcp-connection-panel-narrow.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
            panel.setSize(1100, 760); snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            if (auto output = preview.getSiblingFile("mcp-connection-panel-wide.png").createOutputStream()) { output->setPosition(0); output->truncate(); juce::PNGImageFormat().writeImageToStream(snapshot, *output); }
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
        auto failedConnection = create(ModuleManager::getInstance(), "WebSocket Client", "MCP Disconnected Socket");
        auto* ws = dynamic_cast<WebSocketClientModule*>(mcp.organic.container(failedConnection["handle"].toString()));
        juce::StreamingSocket closedPort; check(closedPort.createListener(0, "127.0.0.1"), "connection failure fixture port");
        auto unavailablePort = closedPort.getBoundPort(); closedPort.close();
        setParameter(ws->serverPath, "127.0.0.1:" + String(unavailablePort)); pumpMessages(100);
        auto connectionDiagnostics = call("chataigne_diagnostics", object({{"scope", failedConnection["handle"]}}));
        check(!ws->isConnected->boolValue() && juce::JSON::toString(connectionDiagnostics).contains("disconnected"), "real connection failure diagnostic evidence");
        call("organic_remove", revisionArgs(ws));
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
        auto conditionData = createFirst(action->cdm.get(), "MCP Condition"); createFirst(action->csmOn.get(), "MCP Consequence");
        auto* condition = dynamic_cast<StandardCondition*>(mcp.organic.container(conditionData["handle"].toString()));
        auto soundCardData = create(ModuleManager::getInstance(), "Sound Card", "MCP Sound Card");
        auto* soundCard = dynamic_cast<AudioModule*>(mcp.organic.container(soundCardData["handle"].toString()));
        soundCard->am.closeAudioDevice(); soundCard->stopTimer();
        check(condition && !condition->comparator, "New standard condition requires input before comparator configuration");
        setParameter(condition->sourceTarget, soundCard->detectedVolume->getControlAddress());
        check(condition->sourceControllable == soundCard->detectedVolume && condition->comparator != nullptr, "Sound Card volume binding creates dynamic comparator");
        auto conditionChildren = call("organic_browse", object({{"scope", conditionData["handle"]}}));
        check(juce::JSON::toString(conditionChildren).contains("Comparator"), "Dynamic comparator is discoverable through assistant tools");
        setParameter(condition->comparator->compareFunction, ">"); setParameter(condition->comparator->reference, 0.5);
        check(condition->comparator->currentFunctionId.toString() == ">" && condition->comparator->reference->floatValue() == .5f, "Condition operator and threshold configured through tools");
        soundCard->detectedVolume->setValue(.5f); check(!condition->comparator->compare(soundCard->detectedVolume), "Greater-than condition excludes threshold");
        soundCard->detectedVolume->setValue(.75f); check(condition->comparator->compare(soundCard->detectedVolume), "Greater-than condition validates above threshold");
        setParameter(condition->sourceTarget, ""); call("organic_remove", revisionArgs(soundCard));
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
        // Author real bindings and record/play back a value through native triggers.
        auto targetArgs = revisionArgs(&parrot->targetsCC); targetArgs.getDynamicObject()->setProperty("type", "Target");
        targetArgs.getDynamicObject()->setProperty("name", "Recorded level");
        auto targetData = call("organic_add_controllable", targetArgs);
        auto* parrotTarget = dynamic_cast<TargetParameter*>(mcp.organic.controllable(targetData["handle"].toString()));
        check(parrotTarget != nullptr, "Parrot target creation"); setParameter(parrotTarget, value->getControlAddress());
        check(parrot->getAllTargets().contains(value), "Parrot target binding");
        auto trigger = [&](Trigger* t) { return call("organic_trigger", object({{"handle", mcp.organic.handle(t)}})); };
        trigger(parrot->startRecordTrigger); setParameter(value, 0.2); pumpMessages(40); setParameter(value, 0.8); pumpMessages(40);
        trigger(parrot->stopRecordTrigger);
        check(parrot->currentRecord && parrot->currentRecord->dataMap[value].size() >= 2, "Parrot records changes through registry controls");
        trigger(parrot->playTrigger); pumpMessages(60);
        check(parrot->playProgression->doubleValue() > 0 && parrot->dataMapPlayIndex[value] >= 2,
            "Parrot playback consumes recorded values");
        if (parrot->status->getValueDataAsEnum<Parrot::Status>() == Parrot::PLAYING) trigger(parrot->stopTrigger);
        auto targetInfo = mcp.organic.describe(targetData["handle"].toString());
        auto removedTarget = call("organic_remove_controllable", object({{"handle", targetInfo["handle"]}, {"revision", targetInfo["revision"]}}));
        check(parrot->targetsCC.controllables.isEmpty(), "user control removal");
        call("organic_undo", object({{"transaction", removedTarget["transaction"]}}));
        check(parrot->getAllTargets().contains(value), "user control removal undo restores target");
        auto dashboardBinding = create(&dashboard->itemManager, "DashboardParameterItem", "MCP Level Control");
        auto* boundItem = dynamic_cast<DashboardParameterItem*>(mcp.organic.container(dashboardBinding["handle"].toString()));
        setParameter(boundItem->target, value->getControlAddress());
        check(boundItem->parameter == value, "dashboard target binding");

        // Diagnose a configured remap without running the chain or emitting output.
        auto* mappingInput = dynamic_cast<StandardMappingInput*>(mapping->im.items[0]);
        check(mappingInput != nullptr, "mapping target input discovery"); setParameter(mappingInput->inputTarget, value->getControlAddress());
        auto* remap = dynamic_cast<SimpleRemapFilter*>(mapping->fm.items[0]);
        setParameter(remap->targetOut, array({10.0, 20.0})); setParameter(value, 0.25); pumpMessages(20);
        mapping->process(false); // Fixture establishes observed state; diagnostics itself must not process.
        auto beforeDiagnostic = value->getValue();
        auto mappingDiagnostic = call("chataigne_diagnostics", object({{"scope", mappingData["handle"]}, {"limit", 200}}));
        check(mappingDiagnostic["processing"].size() == 1 && (bool) mappingDiagnostic["processing"][0]["inputs"][0]["connected"], "diagnostics expose processing dependencies");
        check(mappingDiagnostic["processing"][0]["observedOutputs"].size() == 1
            && std::abs((double) mappingDiagnostic["processing"][0]["observedOutputs"][0]["value"] - 12.5) < 0.01,
            "diagnostics expose unexpected remapped output evidence");
        check(value->getValue() == beforeDiagnostic && !(bool) mappingDiagnostic["processing"][0]["outputs"][0]["commandConfigured"], "diagnostics do not emit or mutate");

        // Discover, create and assign a real built-in command template.
        auto commands = call("chataigne_commands", object({{"module", module["handle"]}}));
        check(commands["commands"].size() >= 2, "module command discovery");
        auto templateData = create(oscModule->templateManager.get(), "Custom Message", "MCP Message Template");
        auto* commandTemplate = dynamic_cast<CommandTemplate*>(mcp.organic.container(templateData["handle"].toString()));
        check(commandTemplate && commandTemplate->sourceDef, "command template authoring");
        auto* outputHandler = mapping->om.items[0]; auto commandArgs = revisionArgs(outputHandler);
        commandArgs.getDynamicObject()->setProperty("module", module["handle"]); commandArgs.getDynamicObject()->setProperty("menuPath", "Templates");
        commandArgs.getDynamicObject()->setProperty("type", commandTemplate->niceName);
        auto assignedCommand = call("chataigne_set_command", commandArgs); check(outputHandler->command != nullptr, "assign discovered template command");
        call("organic_undo", object({{"transaction", assignedCommand["transaction"]}})); check(outputHandler->command == nullptr, "command assignment undo");
        // A script-defined command with an innocuous label cannot bypass Advanced.
        auto* scriptDefinition = CommandDefinition::createDef(oscModule, "Utilities", "Friendly Label", &ScriptCommand::create)->addParam("commandData", object());
        oscModule->defManager->add(scriptDefinition);
        commandArgs = revisionArgs(outputHandler); commandArgs.getDynamicObject()->setProperty("module", module["handle"]);
        commandArgs.getDynamicObject()->setProperty("menuPath", "Utilities"); commandArgs.getDynamicObject()->setProperty("type", "Friendly Label");
        context.grant.access = Access::edit; context.grant.advancedExecution = false;
        rejects([&] { call("chataigne_set_command", commandArgs); }, -32003); context.grant.access = Access::full;
        oscModule->defManager->remove(scriptDefinition);
        auto* consequence = dynamic_cast<BaseCommandHandler*>(action->csmOn->items[0]); check(consequence != nullptr, "action command handler discovery");
        commandArgs = revisionArgs(consequence); commandArgs.getDynamicObject()->setProperty("module", roots["specialModules"][0]["handle"]);
        commandArgs.getDynamicObject()->setProperty("menuPath", ""); commandArgs.getDynamicObject()->setProperty("type", "Set Parameter Value");
        call("chataigne_set_command", commandArgs);
        auto* localCommand = dynamic_cast<GenericControllableCommand*>(consequence->command.get()); check(localCommand != nullptr, "discover and author built-in app command");
        setParameter(localCommand->target, value->getControlAddress()); setParameter(localCommand->value.get(), 0.375);
        context.grant.access = Access::control;
        auto executed = call("chataigne_execute_command", object({{"handle", mcp.organic.handle(consequence)}}));
        check(value->doubleValue() == 0.375 && (bool) executed["executed"] && !(bool) executed["reversible"], "execute authored app command with accurate effect annotation");
        context.grant.access = Access::full;
        // Generic parameter commands must retain the underlying executable gate.
        auto* executableFixture = new ControllableContainer("Executable target fixture"); engine->addChildControllableContainer(executableFixture, true);
        auto* fixtureScript = new Script(); executableFixture->addChildControllableContainer(fixtureScript, true);
        auto* scriptSetting = fixtureScript->addStringParameter("Executable setting", "", "initial");
        setParameter(localCommand->target, scriptSetting->getControlAddress());
        context.grant.access = Access::control;
        rejects([&] { call("chataigne_execute_command", object({{"handle", mcp.organic.handle(consequence)}})); }, -32003);
        context.grant.access = Access::full; setParameter(localCommand->target, value->getControlAddress());
        engine->removeChildControllableContainer(executableFixture);
        auto addVariable = [&](const String& type, const String& name) -> Parameter*
        {
            auto data = create(&cvGroup->values, cvGroup->values.getTypeForControllableType(type), name);
            auto* item = dynamic_cast<GenericControllableItem*>(mcp.organic.container(data["handle"].toString()));
            auto* parameter = item ? dynamic_cast<Parameter*>(item->controllable) : nullptr;
            check(parameter != nullptr, "typed custom variable authoring"); return parameter;
        };
        auto* count = addVariable("Integer", "Count");
        auto* flash = addVariable("Boolean", "Flash");
        auto* label = addVariable("String", "Label");
        auto* tint = addVariable("Color", "Tint");
        auto setRange = [&](Parameter* p, double low, double high)
        {
            auto info = mcp.organic.describe(mcp.organic.handle(p));
            return call("organic_set_range", object({{"handle", info["handle"]}, {"revision", info["revision"]}, {"minimum", low}, {"maximum", high}}));
        };
        setRange(value, 0, 1); setRange(count, 0, 20);
        setParameter(value, 0.2); setParameter(count, 3); setParameter(flash, false); setParameter(label, "Low"); setParameter(tint, array({0.1, 0.2, 0.3, 1.0}));
        createFirst(cvGroup->pm.get(), "Low"); check(cvGroup->pm->items.size() == 1, "preset authoring");
        setParameter(value, 0.8); setParameter(count, 17); setParameter(flash, true); setParameter(label, "High"); setParameter(tint, array({0.8, 0.7, 0.6, 1.0}));
        createFirst(cvGroup->pm.get(), "High");
        check(cvGroup->pm->items.size() == 2 && cvGroup->pm->items[0]->values.getParameterPresetForSource(count)->parameter->intValue() == 3
            && cvGroup->pm->items[1]->values.getParameterPresetForSource(count)->parameter->intValue() == 17, "presets capture distinct values for ranged mixed-type variables");
        commandArgs = revisionArgs(consequence);
        commandArgs.getDynamicObject()->setProperty("module", mcp.organic.handle(CVGroupManager::getInstance()->module.get()));
        commandArgs.getDynamicObject()->setProperty("menuPath", "");
        commandArgs.getDynamicObject()->setProperty("type", "Go to preset");
        call("chataigne_set_command", commandArgs);
        auto* presetCommand = dynamic_cast<CVCommand*>(consequence->command.get());
        check(presetCommand && presetCommand->targetPreset, "preset command authoring");
        auto presetBinding = setParameter(presetCommand->targetPreset, cvGroup->pm->items[0]->getControlAddress());
        check(presetCommand->targetPreset->getTargetContainer() == cvGroup->pm->items[0], "project-wide preset addresses bind to scoped command targets");
        check(presetBinding["targetRoot"].toString() == mcp.organic.handle(CVGroupManager::getInstance())
            && (bool) presetBinding["targetResolved"] && presetBinding["targetHandle"].toString() == mcp.organic.handle(cvGroup->pm->items[0])
            && presetBinding["value"].toString() == cvGroup->pm->items[0]->getControlAddress(CVGroupManager::getInstance()), "target inspection exposes scope and the canonical resolved binding");
        call("organic_undo", object({{"transaction", presetBinding["transaction"]}}));
        check(presetCommand->targetPreset->getTargetContainer() == nullptr, "scoped target normalization remains undoable");
        setParameter(presetCommand->targetPreset, cvGroup->pm->items[0]->getControlAddress(CVGroupManager::getInstance()).substring(1));
        check(presetCommand->targetPreset->getTargetContainer() == cvGroup->pm->items[0], "relative container addresses accept omitted leading slash");
        rejects([&] { setParameter(presetCommand->targetPreset, state["address"]); }, -32602);
        rejects([&] { setParameter(presetCommand->targetPreset, cvGroup->getControlAddress()); }, -32602);
        rejects([&] { setParameter(presetCommand->targetPreset, "/"); }, -32602);
        check(presetCommand->targetPreset->getTargetContainer() == cvGroup->pm->items[0], "out-of-scope target rejection preserves existing binding");
        auto scopedTarget = call("organic_inspect", object({{"handle", mcp.organic.handle(consequence)}, {"path", "command/targetPreset"}}));
        check(scopedTarget["handle"] == presetBinding["handle"], "dynamic command targets can be inspected by a reused relative path");
        rejects([&] { call("organic_inspect", object({{"handle", mcp.organic.handle(consequence)}, {"path", "command/missing"}})); }, -32602);
        rejects([&] { call("organic_inspect", object({{"handle", mcp.organic.handle(consequence)}, {"path", "/"}})); }, -32602);
        auto watch = call("chataigne_detective_watch", object({{"handle", mcp.organic.handle(value)}}));
        auto observed = call("chataigne_detective_inspect", object({{"handle", watch["handle"]}})); check((bool) observed["readOnly"], "Detective observation");
        auto valueInfo = mcp.organic.describe(mcp.organic.handle(value));
        auto set = object({{"handle", valueInfo["handle"]}, {"revision", valueInfo["revision"]}, {"value", 0.5}});
        context.grant.access = Access::inspect;
        rejects([&] { call("organic_set_value", set); }, -32003);
        context.grant.access = Access::control;
        auto edited = call("organic_set_value", set); check(value->doubleValue() == 0.5, "typed parameter edit");
        auto resourceDetails = parseBounded(mcp.registry.read(edited["resourceUri"].toString(), context)["contents"][0]["text"].toString());
        check(resourceDetails["handle"] == edited["handle"] && (double) resourceDetails["value"] == 0.5, "object details resource resolves a discovered handle");
        auto usage = parseBounded(mcp.registry.read("organic://usage", context)["contents"][0]["text"].toString());
        check(usage["workflow"].size() >= 5 && mcp.registry.resourceTemplates()["resourceTemplates"].size() == 1, "usage documentation and resource template discovery");
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
        auto originalVariableCount = cvGroup->values.items.size();
        auto duplicate = call("organic_duplicate", revisionArgs(variableItem));
        check(cvGroup->values.items.size() == originalVariableCount + 1, "duplicate custom variable");
        auto secondGroup = create(CVGroupManager::getInstance(), "", "MCP Destination");
        auto* destination = dynamic_cast<CVGroup*>(mcp.organic.container(secondGroup["handle"].toString()));
        auto move = revisionArgs(mcp.organic.container(duplicate["handle"].toString()));
        auto destinationInfo = mcp.organic.describe(mcp.organic.handle(&destination->values));
        move.getDynamicObject()->setProperty("destination", destinationInfo["handle"]);
        move.getDynamicObject()->setProperty("destinationRevision", destinationInfo["revision"]);
        auto moved = call("organic_move", move);
        check(destination->values.items.size() == 1 && cvGroup->values.items.size() == originalVariableCount, "move between compatible managers");
        check(mcp.organic.container(moved["handle"].toString())->parentContainer == &destination->values, "move returns replacement handle");
        call("organic_undo", object({{"transaction", moved["transaction"]}}));
        check(destination->values.items.isEmpty() && cvGroup->values.items.size() == originalVariableCount + 1, "move undo");
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
        firstInfo = mcp.organic.describe(mcp.organic.handle(value));
        auto repeatedEdit = object({{"handle", firstInfo["handle"]}, {"revision", firstInfo["revision"]}, {"value", 0.6}});
        auto repeated = call("organic_batch", object({{"operations", array({
            object({{"name", "organic_set_value"}, {"arguments", repeatedEdit}}),
            object({{"name", "organic_set_value"}, {"arguments", repeatedEdit.clone()}})})}}));
        check((bool) repeated["success"] && repeated["completed"].size() == 2, "batch advances revisions for its own repeated edits");
        call("organic_undo", object({{"transaction", repeated["transaction"]}})); check(value->doubleValue() == 0.5, "repeated batch edits undo together");
        rejects([&] { call("organic_batch", object({{"operations", array({object({{"name", "organic_set_value"}, {"arguments", repeatedEdit}})})}})); }, -32006);
        check(value->doubleValue() == 0.5, "stale revision at batch entry still rejects without edits");
        auto partial = call("organic_batch", object({{"operations", array({
            object({{"id", "value"}, {"name", "organic_inspect"}, {"arguments", object({{"handle", mcp.organic.handle(value)}})}}),
            object({{"name", "organic_set_value"}, {"target", "value"}, {"arguments", object({{"value", 0.6}})}}),
            object({{"name", "organic_set_value"}, {"target", "value"}, {"arguments", object({{"value", "invalid"}})}})})}}));
        check(!(bool) partial["success"] && partial["completed"].size() == 2 && (int) partial["error"] == -32602 && partial["recovery"].toString().isNotEmpty(), "dependent batch failure reports completed work and structured recovery");
        call("organic_undo", object({{"transaction", partial["transaction"]}})); check(value->doubleValue() == 0.5, "partial dependent batch recovery undo");
        rejects([&] { call("organic_batch", object({{"operations", array({object({{"name", "organic_inspect"}, {"target", "future"}, {"arguments", object()}})})}})); }, -32602);
        rejects([&] { call("organic_undo", object({{"transaction", edited["transaction"]}})); }, -32006);
        auto* file = helper->addFileParameter("Asset", "", directory.getSiblingFile("outside.wav").getFullPathName());
        auto* unresolved = helper->addTargetParameter("Broken reference", ""); unresolved->setValue("/missing/value");
        auto brokenDiagnostics = call("chataigne_diagnostics", object({{"scope", mcp.organic.handle(helper)}}));
        auto diagnosticText = juce::JSON::toString(brokenDiagnostics);
        check(diagnosticText.contains("missing_asset") && diagnosticText.contains("unresolved_target"), "missing asset and broken reference evidence");
        auto fileInfo = mcp.organic.describe(mcp.organic.handle(file));
        context.grant.access = Access::edit; context.grant.folders = {directory.getFullPathName()};
        auto fileEdit = call("organic_set_value", object({{"handle", fileInfo["handle"]}, {"revision", fileInfo["revision"]}, {"value", directory.getChildFile("inside.wav").getFullPathName()}}));
        auto deniedDependency = call("organic_batch", object({{"operations", array({
            object({{"id", "asset"}, {"name", "organic_inspect"}, {"arguments", object({{"handle", fileInfo["handle"]}})}}),
            object({{"name", "organic_set_value"}, {"target", "asset"}, {"arguments", object({{"value", directory.getSiblingFile("outside.wav").getFullPathName()}})}})})}}));
        check(!(bool) deniedDependency["success"] && (int) deniedDependency["error"] == -32003 && deniedDependency["completed"].size() == 1
            && file->getFile() == directory.getChildFile("inside.wav"), "dependent batch targets preserve approved-folder checks without side effects");
        rejects([&] { call("organic_undo", object({{"transaction", fileEdit["transaction"]}})); }, -32003);
        context.grant.access = Access::full;
        call("organic_undo", object({{"transaction", fileEdit["transaction"]}}));
        engine->removeChildControllableContainer(helper);
        auto seq = create(ChataigneSequenceManager::getInstance(), "", "MCP Sequence");
        auto* sequence = dynamic_cast<Sequence*>(mcp.organic.container(seq["handle"].toString())); check(sequence != nullptr, "sequence authoring");
        auto* nativeSequence = dynamic_cast<ChataigneSequence*>(sequence);
        setParameter(nativeSequence->syncOffset, 1.25); setParameter(nativeSequence->mtcFPS, "25");
        check(nativeSequence->syncOffset->doubleValue() == 1.25, "timeline synchronization configuration");
        auto layerData = create(sequence->layerManager.get(), "Audio", "MCP Audio Layer");
        check(juce::Component::getNumCurrentlyModalComponents() == 0, "remote audio layer authoring opens no prerequisite dialog");
        check(juce::JSON::toString(call("chataigne_diagnostics", object({{"scope", layerData["handle"]}}))).contains("audio_output_unassigned"), "unassigned audio output is diagnosed");
        auto* audioLayer = dynamic_cast<AudioLayer*>(mcp.organic.container(layerData["handle"].toString()));
        createFirst(&audioLayer->clipManager, "MCP Audio Clip"); check(audioLayer->clipManager.items.size() == 1, "timeline clip authoring");
        auto media = directory.getChildFile("fixture.wav");
        juce::WavAudioFormat wav; auto mediaStream = media.createOutputStream();
        auto writer = std::unique_ptr<juce::AudioFormatWriter>(wav.createWriterFor(mediaStream.get(), 48000, 1, 16, {}, 0));
        check(writer != nullptr, "media fixture writer"); mediaStream.release(); juce::AudioBuffer<float> silence(1, 4800); silence.clear(); writer->writeFromAudioSampleBuffer(silence, 0, 4800); writer.reset();
        auto* clip = dynamic_cast<AudioLayerClip*>(audioLayer->clipManager.items[0]); check(clip != nullptr, "native audio clip type"); setParameter(clip->filePath, media.getFullPathName());
        for (int attempt = 0; attempt < 100 && (!clip->readerSource || clip->isLoading); ++attempt) pumpMessages(20);
        check(clip->readerSource && std::abs(clip->clipDuration - 0.1) < 0.001, "timeline media assignment loads approved audio");
        auto triggerLayerData = create(sequence->layerManager.get(), "Trigger", "MCP Trigger Layer"); check(sequence->layerManager->items.size() == 2, "timeline layer authoring");
        auto* triggerLayer = dynamic_cast<TriggerLayer*>(mcp.organic.container(triggerLayerData["handle"].toString()));
        setParameter(sequence->totalTime, 12.0);
        auto triggerManagerInfo = revisionArgs(triggerLayer->ttm.get());
        auto triggerTypes = call("organic_types", object({{"handle", triggerManagerInfo["handle"]}}))["types"];
        var triggerOperations = array({});
        for (int i = 0; i < 20; ++i)
        {
            auto id = "step" + String(i + 1); auto arguments = triggerManagerInfo.clone();
            arguments.getDynamicObject()->setProperty("type", triggerTypes[0]["type"]);
            arguments.getDynamicObject()->setProperty("name", "Step " + String(i + 1));
            triggerOperations.append(object({{"id", id}, {"name", "organic_create"}, {"arguments", arguments}}));
            triggerOperations.append(object({{"name", "organic_set_value"}, {"target", id}, {"path", "time"}, {"arguments", object({{"value", (i + 1) * 0.5}})}}));
            triggerOperations.append(object({{"name", "organic_inspect"}, {"target", id}, {"path", "time"}, {"arguments", object()}}));
        }
        auto triggerPreview = call("organic_batch", object({{"preview", true}, {"operations", triggerOperations}}));
        check(!(bool) triggerPreview["valid"] && (int) triggerPreview["deferredValidation"] == 40 && triggerLayer->ttm->items.isEmpty(), "dependent batch preview is read-only and reports deferred validation");
        context.grant.access = Access::inspect;
        rejects([&] { call("organic_batch", object({{"operations", triggerOperations}})); }, -32003);
        check(triggerLayer->ttm->items.isEmpty(), "bulk creation retains access enforcement before editing");
        context.grant.access = Access::full;
        auto triggerBatch = call("organic_batch", object({{"operations", triggerOperations}}));
        if (!(bool) triggerBatch["success"]) std::cerr << "Trigger batch failed at " << triggerBatch["failedIndex"].toString() << ": " << triggerBatch["message"].toString() << '\n';
        check((bool) triggerBatch["success"] && triggerLayer->ttm->items.size() == 20 && triggerBatch["completed"].size() == 60, "twenty triggers are created configured and inspected in one batch");
        for (int i = 0; i < 20; ++i) check((double) triggerBatch["completed"][i * 3 + 2]["result"]["value"] == (i + 1) * 0.5, "each bulk trigger has its requested time");
        // Complete the reported workflow: every time cue loads a real preset.
        juce::StringArray wiringTransactions;
        for (int start = 0; start < 20; start += 10)
        {
            var wiring = array({});
            for (int i = start; i < start + 10; ++i)
            {
                auto* timeTrigger = dynamic_cast<ChataigneTimeTrigger*>(triggerLayer->ttm->items[i]);
                check(timeTrigger != nullptr, "native time trigger consequence manager");
                auto id = "load" + String(i + 1); auto args = revisionArgs(timeTrigger->csm.get());
                args.getDynamicObject()->setProperty("type", "Consequence"); args.getDynamicObject()->setProperty("name", "Load preset");
                wiring.append(object({{"id", id}, {"name", "organic_create"}, {"arguments", args}}));
                wiring.append(object({{"name", "chataigne_set_command"}, {"target", id}, {"arguments", object({
                    {"module", mcp.organic.handle(CVGroupManager::getInstance()->module.get())}, {"menuPath", ""}, {"type", "Go to preset"}})}}));
                wiring.append(object({{"name", "organic_set_value"}, {"target", id}, {"path", "command/targetPreset"},
                    {"arguments", object({{"value", cvGroup->pm->items[i % 2]->getControlAddress()}})}}));
                wiring.append(object({{"name", "organic_set_value"}, {"target", id}, {"path", "command/interpolationTime"}, {"arguments", object({{"value", 0.0}})}}));
                wiring.append(object({{"name", "organic_inspect"}, {"target", id}, {"path", "command/targetPreset"}, {"arguments", object()}}));
            }
            auto wired = call("organic_batch", object({{"operations", wiring}}));
            if (!(bool) wired["success"]) std::cerr << "Preset wiring failed: " << wired["message"].toString() << '\n';
            check((bool) wired["success"] && wired["completed"].size() == 50, "ten time cues bind preset commands and verify their targets in one batch");
            wiringTransactions.add(wired["transaction"].toString());
            for (int i = 0; i < 10; ++i)
                check((bool) wired["completed"][i * 5 + 4]["result"]["targetResolved"], "every preset cue has a resolved target");
        }
        for (int i = 0; i < 20; ++i)
        {
            auto* timeTrigger = dynamic_cast<ChataigneTimeTrigger*>(triggerLayer->ttm->items[i]);
            call("chataigne_execute_command", object({{"handle", mcp.organic.handle(timeTrigger->csm->items[0])}}));
            check(std::abs(value->doubleValue() - (i % 2 == 0 ? 0.2 : 0.8)) < 1.0e-6
                && count->intValue() == (i % 2 == 0 ? 3 : 17) && flash->boolValue() == (i % 2 != 0)
                && label->stringValue() == (i % 2 == 0 ? "Low" : "High"), "every authored time cue actually recalls its mixed-type preset");
        }
        for (int i = wiringTransactions.size(); --i >= 0;)
            call("organic_undo", object({{"transaction", wiringTransactions[i]}}));
        call("organic_undo", object({{"transaction", triggerBatch["transaction"]}}));
        check(triggerLayer->ttm->items.isEmpty(), "all twenty trigger creations and settings undo together");
        auto keyboardData = create(ModuleManager::getInstance(), "Keyboard", "MCP Keyboard");
        auto* keyboard = dynamic_cast<KeyboardModule*>(mcp.organic.container(keyboardData["handle"].toString()));
        check(keyboard && keyboard->keyMap.contains(VK_LCONTROL) && keyboard->keyMap.contains(VK_RCONTROL)
            && keyboard->keyMap.contains('P') && keyboard->keyMap.contains('M'), "keyboard chord inputs are available");
        // Feed native module events without sending any keys to the user's desktop.
        KeyboardHooker::getInstance()->removeListener(keyboard);
        auto keyboardStateData = create(StateManager::getInstance(), "", "MCP Keyboard Controls");
        auto* keyboardState = dynamic_cast<State*>(mcp.organic.container(keyboardStateData["handle"].toString()));
        auto keyAction = [&](const String& name, int key, const String& command, double holdTime)
        {
            auto data = create(keyboardState->pm.get(), "Action", name);
            auto* keyAction = dynamic_cast<Action*>(mcp.organic.container(data["handle"].toString()));
            auto ctrlData = create(keyAction->cdm.get(), "Group", "Either Ctrl");
            auto* ctrlCondition = dynamic_cast<ConditionGroup*>(mcp.organic.container(ctrlData["handle"].toString()));
            check(ctrlCondition != nullptr, "left and right modifier condition group");
            setParameter(ctrlCondition->manager.conditionOperator, "OR");
            for (auto code : {VK_LCONTROL, VK_RCONTROL, key})
            {
                auto* manager = code == key ? keyAction->cdm.get() : &ctrlCondition->manager;
                auto conditionData = createFirst(manager, "Key " + String(code));
                auto* keyCondition = dynamic_cast<StandardCondition*>(mcp.organic.container(conditionData["handle"].toString()));
                setParameter(keyCondition->sourceTarget, keyboard->keyMap[code]->getControlAddress());
                check(keyCondition->comparator != nullptr, "keyboard binding creates boolean comparator");
                setParameter(keyCondition->comparator->compareFunction, "=");
                setParameter(keyCondition->comparator->reference, true);
            }
            setParameter(keyAction->cdm->conditionOperator, "AND"); setParameter(keyAction->cdm->validationTime, holdTime);
            auto consequenceData = createFirst(keyAction->csmOn.get(), "Sequence " + command);
            auto* handler = dynamic_cast<BaseCommandHandler*>(mcp.organic.container(consequenceData["handle"].toString()));
            auto args = revisionArgs(handler);
            args.getDynamicObject()->setProperty("module", mcp.organic.handle(ChataigneSequenceManager::getInstance()->module.get()));
            args.getDynamicObject()->setProperty("menuPath", "Playback"); args.getDynamicObject()->setProperty("type", command);
            call("chataigne_set_command", args);
            auto* sequenceCommand = dynamic_cast<SequenceCommand*>(handler->command.get());
            check(sequenceCommand && sequenceCommand->target, "keyboard consequence assigns a sequence command");
            setParameter(sequenceCommand->target, sequence->getControlAddress());
            check(sequenceCommand->target->getTargetContainer() == sequence, "keyboard command resolves the requested sequence");
            auto inspected = call("organic_inspect", object({{"handle", data["handle"]}, {"includeData", true}}));
            check(inspected["data"].isObject() && keyAction->cdm->items.size() == 2, "keyboard action chord and duration are inspectable");
        };
        keyAction("Ctrl P Play", 'P', "Play Sequence", 0);
        keyAction("Ctrl M Hold Stop", 'M', "Stop Sequence", 1);
        setParameter(keyboardState->active, true);
        keyboard->keyChanged('P', true); pumpMessages(30); check(!sequence->isPlaying->boolValue(), "P alone cannot play the sequence");
        keyboard->keyChanged('P', false); keyboard->keyChanged(VK_LCONTROL, true); pumpMessages(30);
        check(!sequence->isPlaying->boolValue(), "Ctrl alone cannot play the sequence");
        keyboard->keyChanged('P', true); pumpMessages(50); check(sequence->isPlaying->boolValue(), "Ctrl P plays the sequence");
        keyboard->keyChanged('P', false); keyboard->keyChanged('M', true); pumpMessages(300);
        keyboard->keyChanged('M', false); pumpMessages(800); check(sequence->isPlaying->boolValue(), "short Ctrl M press cancels the stop timer");
        keyboard->keyChanged('M', true); pumpMessages(550); keyboard->keyChanged(VK_LCONTROL, false); pumpMessages(650);
        check(sequence->isPlaying->boolValue(), "releasing Ctrl cancels the chord's continuous one-second hold");
        keyboard->keyChanged(VK_RCONTROL, true); pumpMessages(550);
        check(sequence->isPlaying->boolValue(), "Ctrl M does not stop before one second");
        pumpMessages(650); check(!sequence->isPlaying->boolValue() && sequence->currentTime->doubleValue() == 0, "Ctrl M held for one second stops and resets the sequence");
        keyboard->keyChanged('M', false); keyboard->keyChanged(VK_RCONTROL, false);
        call("organic_remove", revisionArgs(keyboardState)); call("organic_remove", revisionArgs(keyboard));
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
        key = revisionArgs(automation); key.getDynamicObject()->setProperty("position", 1.0); key.getDynamicObject()->setProperty("value", 1.0);
        call("timeline_add_key", key); setParameter(automation->items[0]->easingType, "Linear");
        check(std::abs(automation->getValueAtPosition(0.75) - 0.75) < 0.01, "automation interpolation through typed controls");
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
        rejects([&] { mcp.registry.read("organic://object/" + juce::URL::addEscapeChars(oldRoot, true), context); }, -32005);
        call("chataigne_open", object({{"path", project.getFullPathName()}, {"discardChanges", true}}));
        check(ModuleManager::getInstance()->items.size() == 1, "module save/reload");
        check(CVGroupManager::getInstance()->items.size() == 1, "custom variables save/reload");
        check(StateManager::getInstance()->items.size() == 1, "state save/reload");
        check(ChataigneSequenceManager::getInstance()->items.size() == 1, "sequence save/reload");
        auto selection = directory.getChildFile("roundtrip.lilnut");
        auto selectedGroup = mcp.organic.handle(CVGroupManager::getInstance()->items[0]);
        call("chataigne_export_selection", object({{"handles", array({selectedGroup})}, {"path", selection.getFullPathName()}}));
        auto imported = call("chataigne_import", object({{"path", selection.getFullPathName()}}));
        check(CVGroupManager::getInstance()->items.size() == 2, "native selection export/import round trip");
        call("organic_undo", object({{"transaction", imported["transaction"]}})); check(CVGroupManager::getInstance()->items.size() == 1, "selection import undo");
        auto* configuredGroup = CVGroupManager::getInstance()->items[0]; auto configuration = revisionArgs(configuredGroup);
        auto configuredData = configuredGroup->getJSONData().clone(); configuredData.getDynamicObject()->setProperty("niceName", "MCP Configured Group");
        configuration.getDynamicObject()->setProperty("data", configuredData);
        context.grant.access = Access::edit; context.grant.advancedExecution = true;
        rejects([&] { call("chataigne_configure", configuration); }, -32003); context.grant.access = Access::full;
        auto configured = call("chataigne_configure", configuration);
        check(configuredGroup->niceName == "MCP Configured Group" && (bool) configured["irreversibleEffects"] && !(bool) configured["reversible"]
            && (bool) mcp.registry.find("chataigne_configure").descriptor()["annotations"]["openWorldHint"], "arbitrary configuration retains Full gate and reports irreversible external effects");
        for (int iteration = 0; iteration < 8; ++iteration)
        {
            auto stale = mcp.organic.handle(CVGroupManager::getInstance()->items[0]);
            call("chataigne_new", object({{"discardChanges", true}}));
            call("chataigne_open", object({{"path", project.getFullPathName()}, {"discardChanges", true}}));
            rejects([&] { mcp.organic.container(stale); }, -32005);
        }
        check(CVGroupManager::getInstance()->items.size() == 1, "repeated project loads preserve authoring data");
        auto* largeTree = new ControllableContainer("Large tree fixture"); engine->addChildControllableContainer(largeTree, true);
        for (int index = 0; index < 5000; ++index) largeTree->addFloatParameter("Value " + String(index), "", 0);
        auto scope = mcp.organic.handle(largeTree); int seen = 0; auto browseStart = juce::Time::getMillisecondCounterHiRes();
        do { auto page = call("organic_browse", object({{"scope", scope}, {"offset", seen}, {"limit", 200}})); seen += page["items"].size(); if (!page.hasProperty("nextOffset")) break; } while (seen < 6000);
        check(seen == 5000 && juce::Time::getMillisecondCounterHiRes() - browseStart < 30000, "large control tree pagination is bounded and complete");
        auto raced = mcp.organic.describe(mcp.organic.handle(largeTree->controllables[0]));
        dynamic_cast<Parameter*>(largeTree->controllables[0])->setValue(1); // A concurrent native UI edit.
        rejects([&] { call("organic_set_value", object({{"handle", raced["handle"]}, {"revision", raced["revision"]}, {"value", 0.5}})); }, -32006);
        engine->removeChildControllableContainer(largeTree);
        std::cout << "Passed " << checks << " MCP authoring checks\n";
    }
    catch (const std::exception& error) { std::cerr << "MCP authoring failed: " << error.what() << '\n'; result = 1; }
    app.engine.reset(); directory.deleteRecursively(); return result;
}
}
int main(int argc, char** argv)
{
#if ORGANICUI_USE_MCP && ORGANICUI_USE_ASSISTANT
    if (argc == 2 && juce::String(argv[1]) == "--assistant-workflow-smoke") return mcp_test::assistantWorkflowSmoke();
    if (argc == 2 && juce::String(argv[1]) == "--desktop-connection")
    {
        juce::ScopedJuceInitialiser_GUI runtime;
        mcp_test::McpTestApplication app; app.engine.reset(new ChataigneEngine());
        auto* engine = static_cast<ChataigneEngine*>(app.engine.get()); auto& mcp = *engine->mcp;
        auto directory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("Chataigne/MCP/Desktop");
        try
        {
            organic::assistant::ProtectedStore storage(directory); auto record = storage.read("bridge-credential");
            try { mcp.permissions->authenticate(record["token"].toString()); }
            catch (...) { record = organic::mcp::object({{"token", mcp.permissions->create("Claude Desktop", organic::mcp::Access::inspect).second}}); storage.write("bridge-credential", record); }
            auto stop = directory.getChildFile("stop"); stop.deleteFile();
            mcp.port->setValue(42002); mcp.enabled->setValue(true);
            if (!mcp.server->isRunning()) { std::cerr << "Cannot start Chataigne MCP validation instance\n"; return 1; }
            std::cout << "Chataigne engine ready for Claude Desktop at http://127.0.0.1:42002/mcp\n" << std::flush;
            auto deadline = juce::Time::getMillisecondCounterHiRes() + 900000;
            int reported = 0;
            while (!stop.existsAsFile() && juce::Time::getMillisecondCounterHiRes() < deadline)
            {
                mcp_test::pumpMessages(50); auto activity = mcp.server->activity();
                while (reported < activity.size()) { auto event = activity[reported++]; std::cout << "MCP " << event.method << " status " << event.status << '\n' << std::flush; }
            }
            return 0;
        }
        catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
    }
#endif
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
            if (juce::String(argv[1]) == "--subscription-smoke") { provider->cancel(); provider->signIn(false); }
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
            auto passed = state.activity == organic::assistant::Activity::completed && state.transcript.substring(previousLength).contains("[Tool: chataigne_project]");
            std::cout << (passed ? "Passed real subscription-backed assistant smoke test\n" : "Subscription smoke did not complete a local tool call\n");
            return passed ? 0 : 1;
        }
        catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
    }
#endif
    return mcp_test::run();
}
