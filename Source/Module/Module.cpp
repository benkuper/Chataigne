/*
  ==============================================================================

	Module.cpp
	Created: 8 Dec 2016 2:36:02pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "Common/Command/CommandIncludes.h"

Module::Module(const String& name) :
	BaseItem(name, true, true),
	hasInput(true),
	hasOutput(true),
	moduleParams("Parameters"),
	connectionFeedbackRef(nullptr),
	valuesCC("Values"),
	alwaysShowValues(false),
	includeValuesInSave(false),
	customType(""),
	canHandleRouteValues(false)
{
	itemDataType = "Module";
	showWarningInUI = true;

	canInspectChildContainers = true;

	setHasCustomColor(true);
	itemColor->setDefaultValue(BG_COLOR.brighter(.2f));

	inActivityTrigger.reset(new Trigger("IN Activity", "Incoming Activity Signal"));
	outActivityTrigger.reset(new Trigger("OUT Activity", "Outgoing Activity Signal"));

	logIncomingData = addBoolParameter("Log Incoming", "Enable / Disable logging of incoming data for this module", false);
	logIncomingData->hideInEditor = true;

	logOutgoingData = addBoolParameter("Log Outgoing", "Enable / Disable logging of outgoing data for this module", false);
	logOutgoingData->hideInEditor = true;

	moduleParams.saveAndLoadRecursiveData = true;
	addChildControllableContainer(&moduleParams);

	addChildControllableContainer(&valuesCC);
	valuesCC.saveAndLoadRecursiveData = true;
	valuesCC.includeTriggersInSaveLoad = true;

	defManager.reset(new CommandDefinitionManager());

	commandTester.reset(new ModuleCommandTester(this));

	controllableContainers.move(controllableContainers.indexOf(scriptManager.get()), controllableContainers.size() - 1);

	addChildControllableContainer(commandTester.get());

	templateManager.reset(new CommandTemplateManager(this));
	addChildControllableContainer(templateManager.get());

	scriptManager->scriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic", "module"));

	scriptCommanDef.reset(CommandDefinition::createDef(this, "", "Script callback", &ScriptCallbackCommand::create));
}

Module::~Module()
{
	clearItem();
}

void Module::clearItem()
{
	BaseItem::clearItem();

	if (templateManager != nullptr) templateManager->clear();
}

void Module::setupIOConfiguration(bool _hasInput, bool _hasOutput)
{
	if (_hasInput != hasInput) hasInput = _hasInput;
	if (_hasOutput != hasOutput) hasOutput = _hasOutput;

	valuesCC.hideInEditor = !alwaysShowValues && !hasInput && valuesCC.controllables.size() == 0 && valuesCC.controllableContainers.size() == 0;
	commandTester->hideInEditor = defManager->definitions.size() == 0 && !hasOutput;
	templateManager->hideInEditor = defManager->definitions.size() == 0 && !hasOutput;
	moduleListeners.call(&ModuleListener::moduleIOConfigurationChanged);
}

String Module::getHelpID()
{
	return getTypeString();
}

bool Module::isControllableInValuesContainer(Controllable* c)
{
	ControllableContainer* cc = c->parentContainer;
	while (cc != nullptr && cc != &valuesCC) cc = cc->parentContainer;
	return cc == &valuesCC;
}

Array<WeakReference<Controllable>> Module::getValueControllables()
{
	return valuesCC.getAllControllables(true);
}

Array<CommandDefinition*> Module::getCommands(bool includeTemplateCommands)
{
	Array<CommandDefinition*> result;
	for (auto& d : defManager->definitions) result.add(d);
	if (includeTemplateCommands)
	{
		for (auto& td : templateManager->defManager->definitions) result.add(td);
	}

	result.add(scriptCommanDef.get());

	return result;
}

CommandDefinition* Module::getCommandDefinitionFor(StringRef menu, StringRef name)
{
	if (menu == templateManager->menuName) return templateManager->defManager->getCommandDefinitionFor(menu, name);
	if (name == String("Script callback")) return scriptCommanDef.get();
	return defManager->getCommandDefinitionFor(menu, name);
}

PopupMenu Module::getCommandMenu(int offset, CommandContext context)
{
	PopupMenu m = defManager->getCommandMenu(offset, context);
	m.addSeparator();
	templateManager->defManager->addCommandsToMenu(&m, offset + 500, context);
	m.addItem(offset + 401, "Script callback");
	return m;
}

CommandDefinition* Module::getCommandDefinitionForItemID(int itemID)
{
	if (itemID == 400) return scriptCommanDef.get();
	if (itemID >= 500) return templateManager->defManager->definitions[itemID - 500];
	else return defManager->definitions[itemID];
}

void Module::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	if (cc == &valuesCC)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		scriptManager->callFunctionOnAllItems("moduleValueChanged", args);
	}
	else if (cc == &moduleParams)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		scriptManager->callFunctionOnAllItems("moduleParameterChanged", args);
	}

	if (c->type != Controllable::TRIGGER) processDependencies((Parameter*)c);
}

var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", moduleParams.getJSONData()); //keep "params" to avoid conflict with container's parameter

	var templateData = templateManager->getJSONData();
	if (!templateData.isVoid() && templateData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(templateManager->shortName, templateData);

	if (includeValuesInSave) data.getDynamicObject()->setProperty(valuesCC.shortName, valuesCC.getJSONData());

	return data;
}

void Module::loadJSONDataItemInternal(var data)
{
	moduleParams.loadJSONData(data.getProperty("params", var())); //keep "params" to avoid conflict with container's parameter
	if (includeValuesInSave) valuesCC.loadJSONData(data.getProperty(valuesCC.shortName, var()), true);
	templateManager->loadJSONData(data.getProperty(templateManager->shortName, var()), true);
}

void Module::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	setupScriptsFromJSONData(customModuleData); //needs to load after item loadJSON (scriptManager is clearing at load)
}

void Module::setupModuleFromJSONData(var data)
{
	customModuleData = data;
	customType = data.getProperty("name", "");
	setNiceName(data.getProperty("name", ""));


	loadDefaultsParameterValuesForContainer(data.getProperty("defaults", var()), &moduleParams);

	var hideParamsList = data.getProperty("hideDefaultParameters", var());
	if (hideParamsList.isArray())
	{
		for (int i = 0; i < hideParamsList.size(); ++i)
		{
			String pName = hideParamsList[i].toString();
			if (!pName.startsWithChar('/')) pName = "/" + pName;
			Controllable* c = moduleParams.getControllableForAddress(pName, true);
			if (c != nullptr) c->hideInEditor = true;
			ControllableContainer* cc = moduleParams.getControllableContainerForAddress(pName, true);
			if (cc != nullptr) cc->hideInEditor = true;
		}
	}

	customIconPath = File(data.getProperty("modulePath", "")).getChildFile("icon.png");

	createControllablesForContainer(data.getProperty("parameters", var()), &moduleParams);

	moduleParams.hideInEditor = moduleParams.controllables.size() == 0 && moduleParams.controllableContainers.size() == 0;

	alwaysShowValues = data.getProperty("alwaysShowValues", false);
	createControllablesForContainer(data.getProperty("values", var()), &valuesCC);

	Array<WeakReference<Controllable>> valueList = getValueControllables();

	if (!isCurrentlyLoadingData) setupScriptsFromJSONData(data);


	bool valuesAreEmpty = valuesCC.controllables.size() == 0 && valuesCC.controllableContainers.size() == 0;
	bool hInput = data.getProperty("hasInput", valuesAreEmpty ? false : hasInput);
	bool hOutput = data.getProperty("hasOutput", hasOutput);

	if (data.getProperty("hideDefaultCommands", false)) defManager->clear();

	if (data.hasProperty("commands"))
	{
		NamedValueSet commandsData = data.getProperty("commands", var()).getDynamicObject()->getProperties();
		for (auto& cData : commandsData)
		{
			CommandContext context = CommandContext::BOTH;
			if (cData.value.hasProperty("context"))
			{
				String cContext = cData.value.getProperty("context", "both").toString().toLowerCase();
				if (cContext == "action") context = CommandContext::ACTION;
				else if (cContext == "mapping") context = CommandContext::MAPPING;
			}

			defManager->add(CommandDefinition::createDef(this, cData.value.getProperty("menu", ""), cData.name.toString(), &ScriptCommand::create)->addParam("commandData", cData.value));
		}
	}
	setupIOConfiguration(hInput, hOutput);
}

void Module::setupScriptsFromJSONData(var data)
{
	if (!data.hasProperty("scripts")) return;

	Array<var>* scriptData = data.getProperty("scripts", var()).getArray();

	int index = 0;
	for (auto& s : *scriptData)
	{
		Script* script = index < scriptManager->items.size() ? scriptManager->items[index] : scriptManager->addItem(nullptr, var(), false);
		scriptManager->setItemIndex(script, index, false);
		script->filePath->customBasePath = data.getProperty("modulePath", "");
		script->filePath->setControllableFeedbackOnly(true);
		script->filePath->setValue(s.toString());
		script->updateRate->setControllableFeedbackOnly(true);
		script->canBeReorderedInEditor = false;
		script->userCanDuplicate = false;
		script->userCanRemove = false;
		index++;
	}
}

void Module::loadDefaultsParameterValuesForContainer(var data, ControllableContainer* cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto& p : valueProps)
	{
		Parameter* c = static_cast<Parameter*>(cc->getControllableByName(p.name.toString(), true));
		if (c != nullptr)
		{
			c->setValue(p.value);
		}
		else
		{
			ControllableContainer* childCC = cc->getControllableContainerByName(p.name.toString(), true);
			if (childCC != nullptr)
			{
				loadDefaultsParameterValuesForContainer(p.value, childCC);
			}
		}
	};
}

void Module::createControllablesForContainer(var data, ControllableContainer* cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto& p : valueProps)
	{
		if (p.value.getProperty("type", "") == "Container")
		{
			ControllableContainer* childCC = cc->getControllableContainerByName(p.name.toString(), true);

			if (childCC == nullptr)
			{
				int index = p.value.getProperty("index", -1);
				childCC = new ControllableContainer(p.name.toString());
				cc->addChildControllableContainer(childCC, false, index);
				childCC->saveAndLoadRecursiveData = true;
				customModuleContainers.add(childCC);
			}

			childCC->editorIsCollapsed = p.value.getProperty("collapsed", false);
			createControllablesForContainer(p.value, childCC);

		}
		else
		{
			Controllable* c = ControllableFactory::createControllableFromJSON(p.name.toString(), p.value);
			if (c == nullptr) continue;
			cc->addControllable(c);


			if (c->type != Controllable::TRIGGER)
			{
				Parameter* param = (Parameter*)c;
				if (p.value.hasProperty("dependency"))
				{
					var depVar = p.value.getDynamicObject()->getProperty("dependency");
					if (depVar.hasProperty("source") && depVar.hasProperty("value") && depVar.hasProperty("check") && depVar.hasProperty("action"))
					{
						Parameter* sourceP = cc->getParameterByName(depVar.getProperty("source", ""), true);
						if (sourceP != nullptr)
						{
							Dependency* d = new Dependency(sourceP, param, depVar.getProperty("value", 0), depVar.getProperty("check", "").toString(), depVar.getProperty("action", "").toString());
							dependencies.add(d);
						}
						else
						{
							LOGWARNING("Could not find dependency source with name " << depVar.getProperty("source", "").toString());
						}
					}
					else
					{
						LOGWARNING("Dependency definition is not complete, requires source, value, check and action");
					}
				}
			}
		}
	}
}

void Module::onContainerNiceNameChanged()
{
	BaseItem::onContainerNiceNameChanged();
	templateManager->setupDefinitionsFromModule();
}


void Module::processDependencies(Parameter* p)
{
	//Dependencies
	bool changed = false;
	for (auto& d : dependencies)
	{

		if (d->source == p) if (d->process()) changed = true;
	}

	if (changed)
	{
		queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));
	}
}

InspectableEditor* Module::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ModuleEditor(this, isRoot); //temp, should have a proper base module editor
}

ModuleUI* Module::getModuleUI()
{
	return new ModuleUI(this);
}

Array<TargetStepButton *> Module::getTargetLabelForValueControllable(Controllable* c)
{
	Array<TargetStepButton *> result;

	//String label = c->niceName;

	int maxLevels = 3;

	Module* m = ControllableUtil::findParentAs<Module>(c);
	if (m == nullptr) return result;// c->getControlAddress();

	int index = 0;
	ControllableContainer* cc = c->parentContainer;
	while (cc != nullptr && cc != &m->valuesCC && index < maxLevels)
	{
		result.insert(0, new TargetStepButton(cc->niceName+" >", cc));
		//label = cc->niceName + "/" + label;
		cc = cc->parentContainer;
		index++;
	}

	result.insert(0, new TargetStepButton(m->niceName+" >", &m->valuesCC));
	//label = m->niceName + " > " + label;

	result.add(new TargetStepButton(c->niceName, nullptr));
	return result;
}



// DEPENDENCY

Module::Dependency::Dependency(Parameter* source, Parameter* target, var value, CheckType checkType, DepAction depAction)
	: source(source), target(target), value(value), type(checkType), action(depAction)
{
	//DBG("New dependency :" << type << " /" << action);
}


Module::Dependency::Dependency(Parameter* source, Parameter* target, var value, StringRef typeName, StringRef actionName) :
	Dependency(source, target, value, CHECK_NOT_SET, ACTION_NOT_SET)
{
	type = (CheckType)jmax(checkTypeNames.indexOf(typeName), 0);
	action = (DepAction)jmax(actionNames.indexOf(actionName), 0);
}

bool Module::Dependency::process()
{
	if (target.wasObjectDeleted()) return false;

	bool depIsOk = false;

	var valueToCheck = source->value;
	if (source->type == Controllable::ENUM) valueToCheck = ((EnumParameter*)source)->getValueData();

	switch (type)
	{
	case EQUALS: depIsOk = valueToCheck == value; break;
	case NOT_EQUALS: depIsOk = valueToCheck != value; break;
	case LESS_THAN: depIsOk = valueToCheck < value; break;
	case GREATER_THAN: depIsOk = valueToCheck > value; break;
	default: break;
	}


	bool changed = false;

	switch (action)
	{
	case SHOW:
		changed = target->hideInEditor == depIsOk;
		target->hideInEditor = !depIsOk;
		break;

	case ENABLE:
		changed = target->enabled == !depIsOk;
		target->setEnabled(depIsOk);
		break;
	default: break;
	}

	return changed;
}
