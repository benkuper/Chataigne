#include "ChataigneMcp.h"
#if ORGANICUI_USE_MCP
#include <regex>

using organic::mcp::Access;
using organic::mcp::Context;
using organic::mcp::Error;
using organic::mcp::Operation;
using organic::mcp::Resource;
using organic::mcp::Prompt;
using organic::mcp::Permissions;
using organic::mcp::Server;
using organic::mcp::object;
using organic::mcp::schema;
using organic::mcp::stringSchema;
using organic::mcp::integerSchema;
using organic::mcp::openObjectSchema;
using organic::mcp::inspect;
using organic::mcp::control;
using organic::mcp::edit;
using organic::mcp::advanced;
using organic::mcp::files;
namespace
{
String safeDiagnosticText(const String& text)
{
    auto result = std::regex_replace(text.toStdString(), std::regex("Bearer +[^ \\r\\n]+", std::regex::icase), "Bearer [redacted]");
    result = std::regex_replace(result, std::regex("(password|token|secret|api[_ -]?key)([=: ]+)[^,; \\r\\n]+", std::regex::icase), "$1$2[redacted]");
    return String(result).substring(0, 8192);
}
bool executableCommand(BaseCommand* command, int depth = 0, int multiplexIndex = 0)
{
    if (depth > 16) return true;
    if (auto* generic = dynamic_cast<GenericControllableCommand*>(command))
        if (auto* target = generic->getTargetControllableAtIndex(multiplexIndex))
            for (auto* parent = target->parentContainer.get(); parent; parent = parent->parentContainer.get())
            {
                if (dynamic_cast<Script*>(parent) || dynamic_cast<OSModule*>(parent)) return true;
                if (auto* nested = dynamic_cast<BaseCommand*>(parent)) if (executableCommand(nested, depth + 1, multiplexIndex)) return true;
            }
    return dynamic_cast<OSExecCommand*>(command) || dynamic_cast<OSPowerCommand*>(command)
        || dynamic_cast<GenericScriptCommand*>(command) || dynamic_cast<ScriptCommand*>(command)
        || dynamic_cast<ScriptCallbackCommand*>(command);
}
bool executableDefinition(CommandDefinition* definition, Module* module, int depth = 0)
{
    if (!definition || !module || depth > 16) return true;
    if (dynamic_cast<OSModule*>(module) || module->customType.isNotEmpty() || definition->params.hasProperty("commandData")
        || definition->commandType.containsIgnoreCase("script") || definition->menuPath.containsIgnoreCase("script")) return true;
    if (definition->params.hasProperty("template"))
    {
        for (auto* item : module->templateManager->items)
            if (item->shortName == definition->params["template"].toString()) return executableDefinition(item->sourceDef, module, depth + 1);
        return true;
    }
    return false;
}
class SetCommandAction : public juce::UndoableAction
{
public:
    SetCommandAction(BaseCommandHandler* target, CommandDefinition* definition)
        : target(target), next(definition), previous(target->commandDefinition), before(target->command ? target->command->getJSONData().clone() : var()) {}
    bool perform() override { auto* h = dynamic_cast<BaseCommandHandler*>(target.get()); if (!h || next == nullptr) return false; h->setCommand(next.get()); return true; }
    bool undo() override { auto* h = dynamic_cast<BaseCommandHandler*>(target.get()); if (!h) return false; h->setCommand(previous.get()); if (h->command && before.getDynamicObject() != nullptr) h->command->loadJSONData(before); return true; }
private:
    juce::WeakReference<ControllableContainer> target;
    juce::WeakReference<CommandDefinition> next, previous;
    var before;
};
}
ChataigneMcp::ChataigneMcp(ChataigneEngine& e)
    : ControllableContainer("MCP"), organic(registry, e), timeline(organic), engine(e)
{
    enabled = addBoolParameter("Enabled", "Allow authenticated MCP clients to connect to this running instance.", false);
    port = addIntParameter("Port", "Dedicated authenticated loopback HTTP port.", 42002, 1, 65535);
    status = addStringParameter("Status", "MCP connection status.", "Disabled"); status->setControllableFeedbackOnly(true);
    organic.parameterCapabilities = [](Controllable* p)
    {
        unsigned capabilities = edit;
        for (auto* parent = p->parentContainer.get(); parent; parent = parent->parentContainer.get())
        {
#if ORGANICUI_USE_ASSISTANT
            if (dynamic_cast<organic::assistant::Controller*>(parent)) capabilities |= advanced;
#endif
            if (dynamic_cast<Script*>(parent)) capabilities |= advanced;
            if (auto* command = dynamic_cast<BaseCommand*>(parent)) if (executableCommand(command)) capabilities |= advanced;
            if (auto* commandTemplate = dynamic_cast<CommandTemplate*>(parent)) if (executableDefinition(commandTemplate->sourceDef, commandTemplate->module)) capabilities |= advanced;
            if (auto* values = dynamic_cast<GenericControllableManager*>(parent))
                if (dynamic_cast<CVGroup*>(values->parentContainer.get())) capabilities = control;
        }
        return capabilities;
    };
    organic.creationCapabilities = [](ControllableContainer* parent, const String& type)
    {
        unsigned capabilities = edit;
        if (dynamic_cast<ScriptManager*>(parent) || type.containsIgnoreCase("script")) capabilities |= advanced;
        if (auto* manager = dynamic_cast<ModuleManager*>(parent))
            if (auto* definition = manager->factory->getDefinitionForType(type)) if (definition->isCustomModule) capabilities |= advanced;
        if (auto* manager = dynamic_cast<CommandTemplateManager*>(parent))
            for (auto* base : manager->factory.defs) if (base->type == type)
                if (auto* definition = dynamic_cast<Factory<CommandTemplate>::Definition*>(base))
                    if (executableDefinition(manager->module->getCommandDefinitionFor(definition->params["menuPath"].toString(), definition->params["commandType"].toString()), manager->module)) capabilities |= advanced;
        return capabilities;
    };
    organic.executionCapabilities = [](Controllable* target)
    {
        unsigned capabilities = control;
#if ORGANICUI_USE_ASSISTANT
        if (dynamic_cast<organic::assistant::Controller*>(target->parentContainer.get())) capabilities |= advanced;
#endif
        if (auto* handler = dynamic_cast<BaseCommandHandler*>(target->parentContainer.get())) if (executableCommand(handler->command.get(), 0, handler->getPreviewIndex())) capabilities |= advanced;
        if (auto* command = dynamic_cast<BaseCommand*>(target->parentContainer.get())) if (executableCommand(command, 0, command->getPreviewIndex())) capabilities |= advanced;
        if (dynamic_cast<OSModule*>(target->parentContainer.get())) capabilities |= advanced;
        return capabilities;
    };
    organic.hasExecutableBehavior = [](ControllableContainer* cc)
    {
        if (dynamic_cast<Script*>(cc)) return true;
        if (auto* command = dynamic_cast<BaseCommand*>(cc)) return executableCommand(command);
        if (auto* handler = dynamic_cast<BaseCommandHandler*>(cc)) return executableCommand(handler->command.get());
        if (auto* commandTemplate = dynamic_cast<CommandTemplate*>(cc)) return executableDefinition(commandTemplate->sourceDef, commandTemplate->module);
        return false;
    };
    organic.validateCreation = [](ControllableContainer* parent, const String& type, const Context& c)
    {
        if (auto* manager = dynamic_cast<ModuleManager*>(parent))
            if (auto* definition = manager->factory->getDefinitionForType(type)) if (definition->isCustomModule) c.requireFile(definition->moduleFolder);
    };
    auto directory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("Chataigne/MCP");
    try
    {
        permissions = std::make_unique<Permissions>(directory.getChildFile("clients.json"));
        server = std::make_unique<Server>(registry, *permissions, "Chataigne", ProjectInfo::versionString);
        connectionSettings = std::make_unique<organic::mcp::ConnectionSettings>(*permissions, *server, [this] { return port->intValue(); });
        addChildControllableContainer(connectionSettings.get());
        organic.documentInvalidated = [this] { if (server) server->invalidateDocument(); };
    }
    catch (const Error& error) { status->setValue(String(error.what())); }
    registerOperations();
#if ORGANICUI_USE_ASSISTANT
    if (permissions)
    {
        try
        {
            assistant = std::make_unique<organic::assistant::ChatGptProvider>(registry, *permissions,
                directory.getSiblingFile("Assistant"), "Chataigne", [this] { return organic.generation(); },
                [this] { return assistantController && !assistantController->includeProjectContext->boolValue() ? object() : projectSummary(); });
            assistantController = std::make_unique<organic::assistant::Controller>(*assistant);
            engine.addChildControllableContainer(assistantController.get());
            GlobalSettings::getInstance()->addChildControllableContainer(&assistantController->settings);
        }
        catch (const Error& error) { status->setValue(String(error.what())); }
    }
#endif
    engine.addEngineListener(this);
    CustomLogger::getInstance()->addLogListener(this);
    GlobalSettings::getInstance()->addChildControllableContainer(this);
}
ChataigneMcp::~ChataigneMcp()
{
#if ORGANICUI_USE_ASSISTANT
    if (assistantController)
    {
        engine.removeChildControllableContainer(assistantController.get());
        if (auto* settings = GlobalSettings::getInstanceWithoutCreating()) settings->removeChildControllableContainer(&assistantController->settings);
        assistantController.reset();
    }
    assistant.reset();
#endif
    if (server) server->stop();
    if (connectionSettings) { removeChildControllableContainer(connectionSettings.get()); connectionSettings.reset(); }
    cancelPendingUpdate();
    engine.removeEngineListener(this);
    if (auto* logger = CustomLogger::getInstanceWithoutCreating()) logger->removeLogListener(this);
    if (auto* settings = GlobalSettings::getInstanceWithoutCreating()) settings->removeChildControllableContainer(this);
}
void ChataigneMcp::onContainerParameterChanged(Parameter* p)
{
    if ((p != enabled && p != port) || !server) return;
    if (!enabled->boolValue()) { server->stop(); status->setValue("Disabled"); return; }
    auto result = server->start(port->intValue());
    status->setValue(result.wasOk() ? "Listening at http://127.0.0.1:" + String(port->intValue()) + "/mcp" : result.getErrorMessage());
}
void ChataigneMcp::startLoadFile()
{
    if (server) server->invalidateDocument();
    if (juce::MessageManager::getInstance()->isThisTheMessageThread()) organic.invalidateDocument(); else triggerAsyncUpdate();
}
void ChataigneMcp::engineCleared() { startLoadFile(); }
void ChataigneMcp::handleAsyncUpdate() { organic.invalidateDocument(); }
void ChataigneMcp::newMessage(const CustomLogger::LogEvent& event)
{
    recentLogs.add(object({{"time", event.time.toISO8601(true)}, {"severity", event.severityName},
        {"source", safeDiagnosticText(event.source)}, {"message", safeDiagnosticText(event.content)}}));
    if (recentLogs.size() > 200) recentLogs.remove(0);
}
var ChataigneMcp::projectSummary()
{
    juce::Array<var> specialModules;
    for (auto* module : {static_cast<Module*>(engine.module.get()), static_cast<Module*>(engine.multiplexModule.get()),
        static_cast<Module*>(StateManager::getInstance()->module.get()), static_cast<Module*>(ChataigneSequenceManager::getInstance()->module.get()),
        static_cast<Module*>(CVGroupManager::getInstance()->module.get())})
        specialModules.add(object({{"name", module->niceName}, {"handle", organic.handle(module)}}));
    organic.ensureReady();
    return object({{"name", engine.getDocumentTitle()}, {"file", engine.getFile().getFullPathName()},
        {"modified", engine.hasChangedSinceSaved()}, {"generation", organic.generation()},
        {"root", organic.handle(&engine)}, {"modules", organic.handle(ModuleManager::getInstance())},
        {"states", organic.handle(StateManager::getInstance())}, {"sequences", organic.handle(ChataigneSequenceManager::getInstance())},
        {"routing", organic.handle(ModuleRouterManager::getInstance())}, {"variables", organic.handle(CVGroupManager::getInstance())},
        {"dashboards", organic.handle(DashboardManager::getInstance())}, {"parrot", organic.handle(ParrotManager::getInstance())},
        {"detective", organic.handle(Detective::getInstance())}, {"specialModules", specialModules}});
}
var ChataigneMcp::diagnostics(const var& args, Context& context)
{
    organic.ensureReady();
    auto* scope = args["scope"].toString().isEmpty() ? static_cast<ControllableContainer*>(&engine) : organic.container(args["scope"].toString());
    int offset = (int) args.getProperty("offset", 0), limit = (int) args.getProperty("limit", 100);
    juce::Array<var> findings, processing; juce::Array<juce::WeakReference<ControllableContainer>> pending; pending.add(scope);
    int visited = 0, inspected = 0;
    for (int i = 0; i < pending.size() && visited < 10000; ++i)
    {
        context.check(); auto* cc = pending[i].get(); if (!cc) continue;
        for (auto child : cc->controllableContainers) pending.add(child);
        if (visited++ < offset) continue;
        if (inspected++ >= limit) break;
        auto warning = cc->getWarningMessage();
        if (warning.isNotEmpty()) findings.add(object({{"kind", "warning"}, {"handle", organic.handle(cc)}, {"address", cc->getControlAddress()}, {"evidence", safeDiagnosticText(warning)}}));
        if (auto* script = dynamic_cast<Script*>(cc)) if (script->state == Script::SCRIPT_ERROR)
            findings.add(object({{"kind", "script_error"}, {"handle", organic.handle(cc)}, {"evidence", safeDiagnosticText(script->getWarningMessage())}}));
        if (auto* module = dynamic_cast<Module*>(cc)) if (module->connectionFeedbackRef && !module->connectionFeedbackRef->boolValue())
            findings.add(object({{"kind", "disconnected"}, {"handle", organic.handle(cc)}, {"evidence", "Connection feedback is false"}}));
        if (auto* audio = dynamic_cast<ChataigneAudioLayer*>(cc)) if (!audio->audioModule)
            findings.add(object({{"kind", "audio_output_unassigned"}, {"handle", organic.handle(cc)}, {"evidence", "Audio layer has no Sound Card module; create or assign one before sound output"}}));
        if (auto* mapping = dynamic_cast<Mapping*>(cc))
        {
            const juce::ScopedTryLock lock(mapping->mappingLock);
            auto snapshot = object({{"kind", "mapping"}, {"handle", organic.handle(mapping)}, {"snapshotAvailable", lock.isLocked()}});
            if (lock.isLocked())
            {
                juce::Array<var> inputs, filters, outputs, values;
                for (auto* input : mapping->im.items) if (inputs.size() < 32)
                {
                    auto entry = object({{"handle", organic.handle(input)}, {"connected", input->inputReference != nullptr}});
                    if (auto* source = input->inputReference.get()) entry.getDynamicObject()->setProperty("source", organic.describe(organic.handle(source)));
                    inputs.add(entry);
                }
                for (auto* filter : mapping->fm.items) if (filters.size() < 32) filters.add(organic.describe(organic.handle(filter)));
                for (auto* output : mapping->om.items) if (outputs.size() < 32)
                    outputs.add(object({{"handle", organic.handle(output)}, {"commandConfigured", output->command != nullptr}, {"forceDisabled", output->forceDisabled}}));
                for (auto* value : mapping->outValuesCC.controllables) if (values.size() < 32) values.add(organic.describe(organic.handle(value)));
                snapshot.getDynamicObject()->setProperty("inputs", inputs); snapshot.getDynamicObject()->setProperty("filters", filters);
                snapshot.getDynamicObject()->setProperty("outputs", outputs); snapshot.getDynamicObject()->setProperty("observedOutputs", values);
                snapshot.getDynamicObject()->setProperty("truncated", mapping->im.items.size() > 32 || mapping->fm.items.size() > 32 || mapping->om.items.size() > 32 || mapping->outValuesCC.controllables.size() > 32);
            }
            processing.add(snapshot);
        }
        for (auto* parameter : cc->controllables)
        {
            if (auto* target = dynamic_cast<TargetParameter*>(parameter))
                if (target->getValue().toString().isNotEmpty() && target->target == nullptr && target->targetContainer == nullptr)
                    findings.add(object({{"kind", "unresolved_target"}, {"handle", organic.handle(parameter)}, {"evidence", target->getValue()}}));
            if (auto* file = dynamic_cast<FileParameter*>(parameter))
            {
                auto path = file->getFile();
                if (file->getValue().toString().isNotEmpty() && context.grant.allowsFile(path) && !path.exists())
                    findings.add(object({{"kind", "missing_asset"}, {"handle", organic.handle(parameter)}, {"evidence", path.getFullPathName()}}));
            }
        }
    }
    auto result = object({{"findings", findings}, {"processing", processing}, {"logs", recentLogs}, {"readOnly", true}, {"evidenceTime", juce::Time::getCurrentTime().toISO8601(true)}});
    if (inspected > limit || visited >= 10000) result.getDynamicObject()->setProperty("nextOffset", offset + limit);
    return result;
}
void ChataigneMcp::registerOperations()
{
    auto add = [this](String name, String description, var input, unsigned capabilities,
        std::function<var(const var&, Context&)> execute, std::function<void(const var&, const Context&)> prevalidate = {}, bool reversible = false, bool external = false)
    {
        Operation op; op.name = name; op.description = description; op.input = input; op.output = openObjectSchema();
        op.capabilities = capabilities; op.execute = execute; op.prevalidate = prevalidate; op.reversible = reversible; op.externalEffects = external; registry.add(op);
    };
    add("chataigne_project", "Inspect the project and discover each workflow's root handle.", schema(), inspect, [this](const var&, Context&) { return projectSummary(); });
    add("chataigne_diagnostics", "Read observed warnings, connections, missing approved assets, unresolved targets, script failures, and recent logs. Does not emit test output.",
        schema(object({{"scope", stringSchema()}, {"offset", integerSchema(0, 10000)}, {"limit", integerSchema(1, 200)}})), inspect,
        [this](const var& args, Context& c) { return diagnostics(args, c); });
    add("chataigne_add_transition", "Create a state transition between two existing states in one undo transaction. Configure its conditions and consequences through the returned handle.",
        schema(object({{"source", stringSchema()}, {"destination", stringSchema()}, {"sourceRevision", integerSchema(1, INT32_MAX)}, {"destinationRevision", integerSchema(1, INT32_MAX)}}),
            organic::mcp::array({"source", "destination", "sourceRevision", "destinationRevision"})), edit,
        [this](const var& args, Context&) { return organic.transaction("Add state transition", [&]
        {
            auto* source = dynamic_cast<State*>(organic.container(args["source"].toString()));
            auto* destination = dynamic_cast<State*>(organic.container(args["destination"].toString()));
            auto* transition = StateManager::getInstance()->stm.addItemFromData(object({{"sourceState", source->shortName}, {"destState", destination->shortName}}), true);
            if (!transition) throw Error(-32000, "Could not create state transition");
            return organic.describe(organic.handle(transition));
        }); },
        [this](const var& args, const Context&)
        {
            auto* source = dynamic_cast<State*>(organic.container(args["source"].toString()));
            auto* destination = dynamic_cast<State*>(organic.container(args["destination"].toString()));
            if (!source || !destination || source == destination) throw Error(-32602, "Transition requires two distinct states");
            if ((juce::int64) organic.describe(args["source"].toString())["revision"] != (juce::int64) args["sourceRevision"]
                || (juce::int64) organic.describe(args["destination"].toString())["revision"] != (juce::int64) args["destinationRevision"])
                throw Error(-32006, "State changed since inspection");
            if (StateManager::getInstance()->stm.getItemForSourceAndDest(source, destination)) throw Error(-32602, "Transition already exists");
        }, true);
    auto fileInput = schema(object({{"path", stringSchema()}}), organic::mcp::array({"path"}));
    auto requireFile = [this](const var& args, const Context& c)
    { organic.ensureReady(); if (!juce::File::isAbsolutePath(args["path"].toString())) throw Error(-32602, "Use an absolute file path"); c.requireFile(juce::File(args["path"].toString())); };
    add("chataigne_save", "Save the current project to an approved absolute path. Existing files are replaced.", fileInput, edit | files,
        [this](const var& args, Context&) { auto file = juce::File(args["path"].toString()); auto result = engine.saveDocument(file); if (result.failed()) throw Error(-32000, result.getErrorMessage()); engine.setFile(file); return object({{"saved", true}, {"path", file.getFullPathName()}}); }, requireFile, false, true);
    auto openInput = fileInput.clone(); openInput["properties"].getDynamicObject()->setProperty("discardChanges", object({{"type", "boolean"}}));
    add("chataigne_open", "Open an approved project. Advanced is required because project loading can introduce scripts and system commands. Returns loading status.", openInput, edit | files | advanced,
        [this](const var& args, Context&) { auto file = juce::File(args["path"].toString()); auto result = engine.loadDocumentNoCheck(file); if (result.failed()) throw Error(-32000, result.getErrorMessage()); return object({{"loading", engine.isLoadingFile}, {"generation", organic.generation()}}); },
        [this, requireFile](const var& args, const Context& c) { requireFile(args, c); if (!juce::File(args["path"].toString()).existsAsFile()) throw Error(-32602, "Project file does not exist"); if (engine.hasChangedSinceSaved() && !(bool) args["discardChanges"]) throw Error(-32602, "Project has unsaved changes; save first or explicitly discard them"); }, false, true);
    add("chataigne_new", "Replace the current project with a new project. Unsaved changes require discardChanges=true.", schema(object({{"discardChanges", object({{"type", "boolean"}})}})), edit,
        [this](const var&, Context&) { engine.createNewGraph(); return projectSummary(); },
        [this](const var& args, const Context&) { organic.ensureReady(); if (engine.hasChangedSinceSaved() && !(bool) args["discardChanges"]) throw Error(-32602, "Project has unsaved changes; save first or explicitly discard them"); });
    add("chataigne_export", "Export a selected container's existing JSON project format to an approved file.",
        schema(object({{"handle", stringSchema()}, {"path", stringSchema()}}), organic::mcp::array({"handle", "path"})), edit | files,
        [this](const var& args, Context&) { auto data = organic.container(args["handle"].toString())->getJSONData(); auto target = juce::File(args["path"].toString()); juce::TemporaryFile temp(target); if (!temp.getFile().replaceWithText(juce::JSON::toString(data)) || !temp.overwriteTargetFileWithTemporary()) throw Error(-32000, "Could not export object"); return object({{"exported", true}}); },
        [this, requireFile](const var& args, const Context& c) { requireFile(args, c); organic.container(args["handle"].toString()); }, false, true);
    auto selectionInput = schema(object({{"handles", object({{"type", "array"}, {"items", stringSchema()}, {"minItems", 1}, {"maxItems", 64}})}, {"path", stringSchema()}}), organic::mcp::array({"handles", "path"}));
    add("chataigne_export_selection", "Export top-level modules, variable groups, states, sequences or routers as a native .lilnut selection that chataigne_import can reload.", selectionInput, edit | files,
        [this](const var& args, Context&)
        {
            auto data = object();
            for (auto& handle : *args["handles"].getArray())
            {
                auto* item = organic.container(handle.toString()); auto key = item->parentContainer->shortName;
                auto items = data.getProperty(key, {}); if (!items.isArray()) items = organic::mcp::array({});
                items.append(item->getJSONData()); data.getDynamicObject()->setProperty(key, items);
            }
            auto text = juce::JSON::toString(data);
            if (text.getNumBytesAsUTF8() > 1024 * 1024) throw Error(-32602, "Selection exceeds the import size limit; export smaller selections");
            juce::TemporaryFile temp(juce::File(args["path"].toString()));
            if (!temp.getFile().replaceWithText(text) || !temp.overwriteTargetFileWithTemporary()) throw Error(-32000, "Could not export selection");
            return object({{"exported", true}, {"itemCount", args["handles"].size()}});
        },
        [this, requireFile](const var& args, const Context& c)
        {
            requireFile(args, c); juce::StringArray seen;
            for (auto& handle : *args["handles"].getArray())
            {
                auto* item = dynamic_cast<BaseItem*>(organic.container(handle.toString())); auto* parent = item ? item->parentContainer.get() : nullptr;
                if (!item || !item->isSavable || seen.contains(handle.toString()) || !(dynamic_cast<ModuleManager*>(parent) || dynamic_cast<CVGroupManager*>(parent)
                    || dynamic_cast<StateManager*>(parent) || dynamic_cast<ChataigneSequenceManager*>(parent) || dynamic_cast<ModuleRouterManager*>(parent)))
                    throw Error(-32602, "Selection requires distinct supported top-level items");
                seen.add(handle.toString());
            }
        }, false, true);
    // Loading arbitrary serialized object data can contain subtype-specific file
    // paths and executable behavior. Restricted grants use typed operations.
    add("chataigne_configure", "Load existing-format JSON into an object. Requires Full Access and may run initialization or emit output. Effects are not undoable; restricted profiles use typed operations.",
        schema(object({{"handle", stringSchema()}, {"revision", integerSchema(1, INT32_MAX)}, {"data", openObjectSchema()}}), organic::mcp::array({"handle", "revision", "data"})), edit | advanced,
        [this](const var& args, Context&) { auto* target = organic.container(args["handle"].toString()); target->loadJSONData(args["data"]); engine.changed(); auto result = organic.describe(args["handle"].toString()); result.getDynamicObject()->setProperty("irreversibleEffects", true); result.getDynamicObject()->setProperty("reversible", false); return result; },
        [this](const var& args, const Context& c) { if (c.grant.access != Access::full) throw Error(-32003, "Arbitrary serialized configuration requires Full Access"); if ((juce::int64) organic.describe(args["handle"].toString())["revision"] != (juce::int64) args["revision"]) throw Error(-32006, "Object changed since inspection"); }, false, true);
    add("chataigne_commands", "Discover built-in, community, and template commands on a module, with stable menu path and type selectors.",
        schema(object({{"module", stringSchema()}, {"offset", integerSchema(0, INT32_MAX)}, {"limit", integerSchema(1, 200)}}), organic::mcp::array({"module"})), inspect,
        [this](const var& args, Context&) { auto* module = dynamic_cast<Module*>(organic.container(args["module"].toString())); if (!module) throw Error(-32602, "Handle is not a module"); juce::Array<var> commands; auto definitions = module->getCommands(); int offset = (int) args.getProperty("offset", 0), limit = (int) args.getProperty("limit", 100); for (int i = offset; i < definitions.size() && commands.size() < limit; ++i) { auto* definition = definitions[i]; commands.add(object({{"type", definition->commandType}, {"menuPath", definition->menuPath}, {"context", (int) definition->context}})); } auto result = object({{"commands", commands}, {"total", definitions.size()}}); if (offset + commands.size() < definitions.size()) result.getDynamicObject()->setProperty("nextOffset", offset + commands.size()); return result; });
    auto commandInput = schema(object({{"handle", stringSchema()}, {"revision", integerSchema(1, INT32_MAX)}, {"module", stringSchema()}, {"menuPath", stringSchema()}, {"type", stringSchema()}}), organic::mcp::array({"handle", "revision", "module", "menuPath", "type"}));
    add("chataigne_set_command", "Assign a discovered command to an action, consequence, mapping output, or command handler. Configure returned command parameters separately.", commandInput, edit,
        [this](const var& args, Context&) { return organic.transaction("Assign command", [&] { auto* handler = dynamic_cast<BaseCommandHandler*>(organic.container(args["handle"].toString())); auto* module = dynamic_cast<Module*>(organic.container(args["module"].toString())); auto* definition = module->getCommandDefinitionFor(args["menuPath"].toString(), args["type"].toString()); UndoMaster::getInstance()->performAction("Assign command", new SetCommandAction(handler, definition)); return organic.describe(args["handle"].toString()); }); },
        [this](const var& args, const Context& c)
        {
            auto* handler = dynamic_cast<BaseCommandHandler*>(organic.container(args["handle"].toString())); auto* module = dynamic_cast<Module*>(organic.container(args["module"].toString()));
            if (!handler || !module || (handler->lockedModule && handler->lockedModule != module)) throw Error(-32602, "Incompatible command handler or module");
            if ((juce::int64) organic.describe(args["handle"].toString())["revision"] != (juce::int64) args["revision"]) throw Error(-32006, "Command handler changed since inspection");
            auto* definition = module->getCommandDefinitionFor(args["menuPath"].toString(), args["type"].toString());
            if (!definition || (definition->context != CommandContext::BOTH && definition->context != handler->context)) throw Error(-32602, "Command is not compatible with this context");
            // Selecting executable commands is itself executable authoring. Avoid
            // constructing a command merely to classify it during prevalidation.
            if (executableDefinition(definition, module)) c.require(advanced);
        }, true);
    add("chataigne_execute_command", "Execute an already authored command handler. Direct script or system-command invocation requires Advanced.",
        schema(object({{"handle", stringSchema()}, {"multiplexIndex", integerSchema(0, 100000)}}), organic::mcp::array({"handle"})), control,
        [this](const var& args, Context&) { auto* handler = dynamic_cast<BaseCommandHandler*>(organic.container(args["handle"].toString())); handler->triggerCommand((int) args.getProperty("multiplexIndex", 0)); return object({{"executed", true}, {"reversible", false}}); },
        [this](const var& args, const Context& c)
        {
            auto* handler = dynamic_cast<BaseCommandHandler*>(organic.container(args["handle"].toString()));
            if (!handler || !handler->command) throw Error(-32602, "Handle has no configured command");
            auto index = (int) args.getProperty("multiplexIndex", 0);
            if (executableCommand(handler->command.get(), 0, index)) c.require(advanced);
            if (auto* generic = dynamic_cast<GenericControllableCommand*>(handler->command.get()))
                if (auto* file = dynamic_cast<FileParameter*>(generic->value.get()))
                {
                    auto path = generic->getLinkedValue(file, index).toString();
                    if (!juce::File::isAbsolutePath(path)) throw Error(-32602, "File commands require an absolute linked path");
                    c.requireFile(juce::File(path));
                }
        }, false, true);
    auto scriptInput = schema(object({{"handle", stringSchema()}}), organic::mcp::array({"handle"}));
    auto script = [this](const var& args) { auto* result = dynamic_cast<Script*>(organic.container(args["handle"].toString())); if (!result) throw Error(-32602, "Handle is not a script"); return result; };
    add("chataigne_script_inspect", "Read a script's source, available functions, and error state from its approved file.", scriptInput, inspect | files,
        [script](const var& args, Context&) { auto* s = script(args); return object({{"source", s->filePath->getFile().loadFileAsString()}, {"state", (int) s->state}, {"errors", safeDiagnosticText(s->getWarningMessage())}}); },
        [script](const var& args, const Context& c) { auto file = script(args)->filePath->getFile(); c.requireFile(file); if (!file.existsAsFile() || file.getSize() > 1024 * 1024) throw Error(-32602, "Script file is missing or exceeds the size limit"); });
    auto sourceInput = scriptInput.clone(); sourceInput["properties"].getDynamicObject()->setProperty("source", stringSchema()); sourceInput["required"].getArray()->add("source");
    add("chataigne_script_write", "Replace a script's approved source file and reload it. File writes and execution are irreversible.", sourceInput, edit | advanced | files,
        [script](const var& args, Context&) { auto* s = script(args); auto target = s->filePath->getFile(); juce::TemporaryFile temp(target); if (!temp.getFile().replaceWithText(args["source"].toString()) || !temp.overwriteTargetFileWithTemporary()) throw Error(-32000, "Could not write script"); s->loadScript(); return object({{"written", true}, {"state", (int) s->state}, {"reversible", false}}); },
        [script](const var& args, const Context& c) { c.requireFile(script(args)->filePath->getFile()); }, false, true);
    add("chataigne_script_reload", "Reload an existing script and execute its initialization code.", scriptInput, control | advanced,
        [script](const var& args, Context&) { script(args)->loadScript(); return object({{"reloaded", true}, {"reversible", false}}); },
        [script](const var& args, const Context& c) { c.requireFile(script(args)->filePath->getFile()); }, false, true);
    auto invokeInput = scriptInput.clone();
    invokeInput["properties"].getDynamicObject()->setProperty("function", stringSchema());
    invokeInput["properties"].getDynamicObject()->setProperty("arguments", object({{"type", "array"}, {"items", organic::mcp::anySchema()}, {"maxItems", 32}}));
    invokeInput["required"].getArray()->add("function");
    add("chataigne_script_invoke", "Invoke a declared script function locally. Requires Advanced; execution effects are irreversible.", invokeInput, control | advanced,
        [script](const var& args, Context&) { auto* s = script(args); juce::Array<var> values; if (auto* input = args["arguments"].getArray()) values = *input; juce::Result result = juce::Result::ok(); auto output = s->callFunction(juce::Identifier(args["function"].toString()), values, &result); if (result.failed()) throw Error(-32000, safeDiagnosticText(result.getErrorMessage())); return object({{"result", output}, {"reversible", false}}); },
        [script](const var& args, const Context&) { auto* s = script(args); if (!s->availableFunctions.contains(args["function"].toString())) throw Error(-32602, "Unknown script function"); }, false, true);
    add("chataigne_import", "Import an existing .lilnut selection into the project as one structural undo group. Full Access is required for arbitrary serialized behavior and asset references.", fileInput, edit | files | advanced,
        [this](const var& args, Context&) { return organic.transaction("Import selection", [&] { engine.importSelection(juce::File(args["path"].toString())); auto result = projectSummary(); result.getDynamicObject()->setProperty("irreversibleEffects", true); return result; }); },
        [requireFile](const var& args, const Context& c) { requireFile(args, c); if (c.grant.access != Access::full) throw Error(-32003, "Arbitrary selection import requires Full Access"); auto file = juce::File(args["path"].toString()); if (!file.existsAsFile() || file.getSize() > 1024 * 1024 || organic::mcp::parseBounded(file.loadFileAsString()).getDynamicObject() == nullptr) throw Error(-32602, "Selection must be a bounded JSON object file"); }, true, true);
    add("chataigne_changes", "Compare the current project with its saved file. Returns changed JSON paths, without credential values.", schema(), inspect,
        [this](const var&, Context& c)
        {
            organic.ensureReady(); auto file = engine.getFile();
            if (!file.existsAsFile()) return object({{"modified", engine.hasChangedSinceSaved()}, {"baselineAvailable", false}});
            c.requireFile(file);
            if (file.getSize() > 1024 * 1024) throw Error(-32000, "Saved project exceeds the comparison limit");
            auto before = organic::mcp::parseBounded(file.loadFileAsString()), after = engine.getJSONData();
            juce::Array<var> paths;
            std::function<void(const var&, const var&, const String&, int)> compare;
            compare = [&](const var& a, const var& b, const String& path, int depth)
            {
                c.check(); if (paths.size() >= 200) return;
                if (depth > 24) { paths.add(path); return; }
                if (a.getDynamicObject() && b.getDynamicObject())
                {
                    juce::StringArray names;
                    for (const auto& property : a.getDynamicObject()->getProperties()) names.addIfNotAlreadyThere(property.name.toString());
                    for (const auto& property : b.getDynamicObject()->getProperties()) names.addIfNotAlreadyThere(property.name.toString());
                    for (const auto& name : names)
                    {
                        juce::Identifier key(name);
                        if (paths.size() >= 200) break;
                        if (a.hasProperty(key) != b.hasProperty(key)) paths.add(path + "/" + name);
                        else compare(a[key], b[key], path + "/" + name, depth + 1);
                    }
                }
                else if (a.isArray() && b.isArray())
                { for (int i = 0; i < juce::jmax(a.size(), b.size()) && paths.size() < 200; ++i) { if (i >= a.size() || i >= b.size()) paths.add(path + "/" + String(i)); else compare(a[i], b[i], path + "/" + String(i), depth + 1); } }
                else if (juce::JSON::toString(a, true) != juce::JSON::toString(b, true)) paths.add(path);
            };
            compare(before, after, "", 0);
            return object({{"modified", engine.hasChangedSinceSaved()}, {"baselineAvailable", true}, {"changedPaths", paths}, {"truncated", paths.size() >= 200}});
        });
    add("chataigne_detective_watch", "Observe an existing controllable in Detective without emitting output.",
        schema(object({{"handle", stringSchema()}}), organic::mcp::array({"handle"})), inspect,
        [this](const var& args, Context&) { auto* target = organic.controllable(args["handle"].toString()); auto* detective = Detective::getInstance(); detective->watchControllable(target); auto* watcher = detective->getItemForControllable(target); if (!watcher) throw Error(-32000, "Could not start Detective observation"); return organic.describe(organic.handle(watcher)); });
    add("chataigne_detective_inspect", "Read a bounded snapshot of a Detective watcher's samples.",
        schema(object({{"handle", stringSchema()}, {"limit", integerSchema(1, 200)}}), organic::mcp::array({"handle"})), inspect,
        [this](const var& args, Context&)
        {
            auto* watcher = dynamic_cast<ControllableDetectiveWatcher*>(organic.container(args["handle"].toString()));
            if (!watcher) throw Error(-32602, "Handle is not a Detective watcher");
            juce::Array<var> samples; int limit = (int) args.getProperty("limit", 100);
            bool sensitive = false;
            if (auto* target = watcher->controllable.get()) { auto name = target->shortName.toLowerCase(); sensitive = name.contains("password") || name.contains("token") || name.contains("secret") || name.contains("apikey"); }
            const juce::ScopedLock lock(watcher->data.getLock());
            for (int i = juce::jmax(0, watcher->data.size() - limit); i < watcher->data.size(); ++i)
                samples.add(object({{"time", watcher->data[i]->time}, {"value", sensitive ? var("[redacted]") : watcher->data[i]->val}}));
            return object({{"samples", samples}, {"readOnly", true}});
        });
    registry.add(Resource{"chataigne://project", "Chataigne project", "Workflow roots and project status", [this](Context&) { return projectSummary(); }});
    registry.add(Resource{"chataigne://diagnostics", "Diagnostics", "Observed project diagnostics", [this](Context& c) { return diagnostics(object(), c); }});
    registry.add(Prompt{"chataigne_troubleshoot_connection", "Troubleshoot a connection", "Inspect diagnostics and module connection parameters, identify observed failures and relevant evidence, then propose a correction. Use an explicit control operation only when the user requests output or a test command."});
    registry.add(Prompt{"chataigne_show_readiness", "Review show readiness", "Inspect project diagnostics, missing approved assets, unresolved targets, scripts, module connections, and sequence playback configuration. Report evidence, unchecked scope, and corrective edits. Keep the review read-only."});
}
#endif
