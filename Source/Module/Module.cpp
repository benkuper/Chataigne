/*
  ==============================================================================

    Module.cpp
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Module.h"
#include "Common/Command/CommandFactory.h"
#include "ui/ModuleEditor.h"
#include "Common/Command/Template/CommandTemplate.h"
#include "Module/modules/common/commands/scriptcommands/ScriptCommand.h"
#include "UI/ChataigneAssetManager.h"

Module::Module(const String &name) :
	BaseItem(name, true, true),
	hasInput(true),
	hasOutput(true),
	moduleParams("Parameters"),
	connectionFeedbackRef(nullptr),
	valuesCC("Values"),
	alwaysShowValues(false),
	includeValuesInSave(false),
	commandTester("Command Tester", CommandContext::ACTION),
	templateManager(this),
    canHandleRouteValues(false)

{
	itemDataType = "Module";

	canInspectChildContainers = true;

	logIncomingData = addBoolParameter("Log Incoming", "Enable / Disable logging of incoming data for this module", false);
	logIncomingData->hideInEditor = true;
	logIncomingData->hideInOutliner = true;
	logIncomingData->isTargettable = false;

	logOutgoingData = addBoolParameter("Log Outgoing", "Enable / Disable logging of outgoing data for this module", false);
	logOutgoingData->hideInEditor = true;
	logOutgoingData->hideInOutliner = true;
	logOutgoingData->isTargettable = false;

	inActivityTrigger = addTrigger("IN Activity", "Incoming Activity Signal");
	inActivityTrigger->hideInEditor = true;
	inActivityTrigger->setControllableFeedbackOnly(true);

	outActivityTrigger = addTrigger("OUT Activity", "Outgoing Activity Signal");
	outActivityTrigger->hideInEditor = true;
	outActivityTrigger->setControllableFeedbackOnly(true);

	
	moduleParams.saveAndLoadRecursiveData = true;
	addChildControllableContainer(&moduleParams);
	
	addChildControllableContainer(&valuesCC);
	valuesCC.includeTriggersInSaveLoad = true;


	commandTester.userCanRemove = false;
	commandTester.canBeDisabled = false;
	commandTester.lockedModule = this;
	commandTester.canBeReorderedInEditor = false;

	controllableContainers.move(controllableContainers.indexOf(scriptManager.get()), controllableContainers.size() - 1);

	addChildControllableContainer(&commandTester);

	scriptManager->scriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic","module"));
}

Module::~Module()
{

}

void Module::setupIOConfiguration(bool _hasInput, bool _hasOutput)
{
	if (_hasInput != hasInput) hasInput = _hasInput;
	if (_hasOutput != hasOutput) hasOutput = _hasOutput;
	
	valuesCC.hideInEditor = !alwaysShowValues && !hasInput && valuesCC.controllables.size() == 0 && valuesCC.controllableContainers.size() == 0;
	commandTester.hideInEditor = !hasOutput;
	moduleListeners.call(&ModuleListener::moduleIOConfigurationChanged);
}

String Module::getHelpID()
{
	return getTypeString();
}

bool Module::isControllableInValuesContainer(Controllable * c)
{
	ControllableContainer * cc = c->parentContainer;
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
	for (auto &d : defManager.definitions) result.add(d);
	if (includeTemplateCommands)
	{
		for (auto &td : templateManager.defManager.definitions) result.add(td);
	}

	return result;
}

CommandDefinition * Module::getCommandDefinitionFor(StringRef menu, StringRef name)
{
	if (menu == templateManager.menuName) return templateManager.defManager.getCommandDefinitionFor(menu, name);
	return defManager.getCommandDefinitionFor(menu, name);
}

PopupMenu Module::getCommandMenu(int offset, CommandContext context)
{
	PopupMenu m = defManager.getCommandMenu(offset, context);
	m.addSeparator();
	templateManager.defManager.addCommandsToMenu(&m,offset + 500, context);
	
	return m;
}

CommandDefinition * Module::getCommandDefinitionForItemID(int itemID)
{
	if(itemID >= 500) return templateManager.defManager.definitions[itemID-500];
	else return defManager.definitions[itemID];
}

void Module::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (cc == &valuesCC)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		scriptManager->callFunctionOnAllItems("moduleValueChanged", args);
	} else if (cc == &moduleParams)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		scriptManager->callFunctionOnAllItems("moduleParameterChanged", args);
	}
}

var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", moduleParams.getJSONData());

	var templateData = templateManager.getJSONData();
	if (!templateData.isVoid() && templateData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("templates", templateData);

	if(includeValuesInSave) data.getDynamicObject()->setProperty("values", valuesCC.getJSONData());

	return data;
}

void Module::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	moduleParams.loadJSONData(data.getProperty("params", var()));
	templateManager.loadJSONData(data.getProperty("templates", var()), true);

	if (includeValuesInSave) valuesCC.loadJSONData(data.getProperty("values", var()), true);
}

void Module::setupModuleFromJSONData(var data)
{
	customType = data.getProperty("name","");
	setNiceName(data.getProperty("name",""));

	loadDefaultsParameterValuesForContainer(data.getProperty("defaults", var()), &moduleParams);

	var hideParamsList = data.getProperty("hideDefaultParameters",var());
	if (hideParamsList.isArray())
	{
		for (int i = 0; i < hideParamsList.size(); i++)
		{
			Controllable * c = moduleParams.getControllableByName(hideParamsList[i].toString(), true);
			if (c != nullptr) c->hideInEditor = true;
		}
	}


	createControllablesForContainer(data.getProperty("parameters", var()), &moduleParams);
	createControllablesForContainer(data.getProperty("values", var()), &valuesCC);

	

	Array<WeakReference<Controllable>> valueList = getValueControllables();
	for (auto &c : valueList) c->setControllableFeedbackOnly(true);

	Array<var> * scriptData = data.getProperty("scripts", var()).getArray();
	for (auto &s : *scriptData)
	{
		Script * script = scriptManager->addItem(nullptr,var(),false);
		//DBG("Set script path : " << data.getProperty("modulePath", "").toString() << "/" << s.toString());
		script->filePath->setValue(data.getProperty("modulePath", "").toString() + "/"+s.toString());
	}

	bool valuesAreEmpty = valuesCC.controllables.size() == 0 && valuesCC.controllableContainers.size() == 0;
	bool hInput = data.getProperty("hasInput", valuesAreEmpty ? false : hasInput);
	bool hOutput = data.getProperty("hasOutput", hasOutput);

	if (data.getProperty("hideDefaultCommands", false)) defManager.clear();

	if (data.hasProperty("commands"))
	{
		NamedValueSet commandsData = data.getProperty("commands",var()).getDynamicObject()->getProperties();
		for (auto &cData: commandsData)
		{
			CommandContext context = CommandContext::BOTH;
			if (cData.value.hasProperty("context"))
			{
				String cContext = cData.value.getProperty("context", "both").toString().toLowerCase();
				if (cContext == "action") context = CommandContext::ACTION;
				else if (cContext == "mapping") context = CommandContext::MAPPING;
			}

			defManager.add(CommandDefinition::createDef(this, cData.value.getProperty("menu", ""), cData.name.toString(), &ScriptCommand::create)->addParam("commandData",cData.value));
		}
	}
	setupIOConfiguration(hInput, hOutput);
}

void Module::loadDefaultsParameterValuesForContainer(var data, ControllableContainer * cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto &p : valueProps)
	{
		Parameter * c = static_cast<Parameter *>(cc->getControllableByName(p.name.toString(),true));
		if (c != nullptr)
		{
			c->setValue(p.value);
		} else
		{
			ControllableContainer * childCC = cc->getControllableContainerByName(p.name.toString(), true);
			if (childCC != nullptr)
			{
				loadDefaultsParameterValuesForContainer(p.value, childCC);
			}
		}
	};
}

void Module::createControllablesForContainer(var data, ControllableContainer * cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto &p : valueProps)
	{
		if (p.value.getProperty("type", "") == "Container")
		{
			ControllableContainer * childCC = cc->getControllableContainerByName(p.name.toString(), true);
			if (childCC == nullptr)
			{
				childCC = new ControllableContainer(p.name.toString());
				cc->addChildControllableContainer(childCC);
				customModuleContainers.add(childCC);
			}
			createControllablesForContainer(p.value, childCC);

		} else
		{
			Controllable * c = getControllableForJSONDefinition(p.name.toString(), p.value);
			if (c == nullptr) continue;
			cc->addControllable(c);
		}
	}
}

void Module::onContainerNiceNameChanged()
{
	BaseItem::onContainerNiceNameChanged();
	templateManager.setupTemplateDefinition();
}


Controllable * Module::getControllableForJSONDefinition(const String &name, var def)
{
	String valueType = def.getProperty("type", "").toString();
	Controllable * c = ControllableFactory::createControllable(valueType);
	if (c == nullptr) return nullptr;

	c->setNiceName(name);

	DynamicObject * d = def.getDynamicObject();

	if (c->type != Controllable::TRIGGER)
	{
		if (d->hasProperty("min") && d->hasProperty("max")) ((Parameter *)c)->setRange(d->getProperty("min"), d->getProperty("max"));
		
		if (d->hasProperty("default")) ((Parameter *)c)->setValue(d->getProperty("default"));

		if (c->type == Controllable::ENUM)
		{
			EnumParameter * ep = dynamic_cast<EnumParameter *>(c);

			if (d->hasProperty("options") && d->getProperty("options").isObject())
			{
				NamedValueSet options = d->getProperty("options").getDynamicObject()->getProperties();
				for (auto &o : options)
				{
					ep->addOption(o.name.toString(), o.value);
				}
			} else
			{
				LOG("Options property is not valid : " << d->getProperty("options").toString());
			}
		} else if (c->type == Controllable::FLOAT)
		{
			FloatParameter * ep = dynamic_cast<FloatParameter *>(c);
			if (d->hasProperty("ui"))
			{
				String ui = d->getProperty("ui");
				if (ui == "slider") ep->defaultUI = FloatParameter::SLIDER;
				else if (ui == "stepper") ep->defaultUI = FloatParameter::STEPPER;
				else if (ui == "label") ep->defaultUI = FloatParameter::LABEL;
				else if (ui == "time") ep->defaultUI = FloatParameter::TIME;
			}
		}
	}

	return c;
}

InspectableEditor * Module::getEditor(bool isRoot)
{
	return new ModuleEditor(this, isRoot); //temp, should have a proper base module editor
}

String Module::getTargetLabelForValueControllable(Controllable * c)
{
	ControllableContainer * cc = c->parentContainer;
	while (cc != nullptr)
	{
		Module * m = dynamic_cast<Module *>(cc);
		if (m != nullptr) return m->niceName + ":" + c->niceName;
		cc = cc->parentContainer;
	}

	return c->getControlAddress();
}
