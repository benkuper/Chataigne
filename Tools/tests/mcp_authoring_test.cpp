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
        {
            Permissions uiPermissions(directory.getChildFile("panel-clients.json"));
            Server uiServer(mcp.registry, uiPermissions, "Connection panel test", "1");
            int uiPort = 42009; bool uiEnabled = false;
            ConnectionPanel panel("MCP Connections", uiPermissions, uiServer,
                [&] { return uiEnabled; }, [&](bool enabled) { uiEnabled = enabled; if (enabled) { auto started = uiServer.start(uiPort); check(started.wasOk(), "panel listener start"); } else uiServer.stop(); },
                [&] { return uiPort; }, [&](int value) { uiPort = value; });
            panel.setSize(720, 760);
            auto* name = findComponent<juce::TextEditor>(panel, [](const auto& c) { return c.getName() == "Client name"; });
            auto* profile = findComponent<juce::ComboBox>(panel, [](const auto& c) { return c.getName() == "Access profile"; });
            auto button = [&](const String& text) { auto* found = findComponent<juce::TextButton>(panel, [&](const auto& c) { return c.getButtonText() == text; }); check(found != nullptr, "connection panel button exists"); return found; };
            check(name && profile, "connection panel fields exist"); name->setText("Regression client"); profile->setSelectedId(4, juce::sendNotificationSync);
            button("Create client")->onClick();
            check(uiPermissions.list().size() == 1 && uiPermissions.list()[0].access == Access::full, "panel creates remembered Full Access client");
            auto* enable = findComponent<juce::ToggleButton>(panel, [](const auto& c) { return c.getButtonText() == "Enable authenticated MCP"; });
            enable->setToggleState(true, juce::dontSendNotification); enable->onClick();
            button("Test connection")->onClick();
            auto* resultLabel = findComponent<juce::Label>(panel, [](const auto& c) { return c.getName() == "Connection result"; });
            for (int attempt = 0; attempt < 100 && !resultLabel->getText().contains("succeeded"); ++attempt) pumpMessages(50);
            check(resultLabel->getText().contains("succeeded"), "panel tests authenticated HTTP connection");
            auto snapshot = panel.createComponentSnapshot(panel.getLocalBounds());
            auto preview = juce::File::getCurrentWorkingDirectory().getChildFile("../../Binaries/mcp-connection-panel.png");
            if (auto output = preview.createOutputStream()) juce::PNGImageFormat().writeImageToStream(snapshot, *output);
            button("Rotate credential")->onClick();
            check(uiPermissions.list().size() == 1, "panel rotates existing client");
            button("Revoke client")->onClick(); check(uiPermissions.list().isEmpty(), "panel revokes client");
            uiServer.stop();
        }
        auto module = create(ModuleManager::getInstance(), "OSC", "MCP OSC");
        check(module["type"].toString() == "OSC", "built-in module authoring");
        auto state = create(StateManager::getInstance(), "", "MCP State");
        check(state["handle"].isString(), "state authoring");
        auto group = create(CVGroupManager::getInstance(), "", "MCP Variables");
        auto* cvGroup = dynamic_cast<CVGroup*>(mcp.organic.container(group["handle"].toString())); check(cvGroup != nullptr, "custom variable group authoring");
        auto variable = create(&cvGroup->values, cvGroup->values.getTypeForControllableType("Float"), "Level");
        auto* variableItem = mcp.organic.container(variable["handle"].toString());
        Parameter* value = nullptr; for (auto* c : variableItem->controllables) if (auto* p = dynamic_cast<FloatParameter*>(c)) if (p != dynamic_cast<BaseItem*>(variableItem)->listUISize) { value = p; break; }
        check(value != nullptr, "custom variable parameter discovery");
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
int main() { return mcp_test::run(); }
