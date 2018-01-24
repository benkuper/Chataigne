/*
  ==============================================================================

    Module.cpp
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Module.h"
#include "CommandFactory.h"
#include "ModuleEditor.h"



Module::Module(const String &name) :
	BaseItem(name, true, true),
	hasInput(true),
	hasOutput(true),
	moduleParams("Parameters"),
	valuesCC("Values"),
	commandTester("Command Tester", CommandContext::ACTION),
	canHandleRouteValues(false)
{
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
	inActivityTrigger->isControllableFeedbackOnly = true;

	outActivityTrigger = addTrigger("OUT Activity", "Outgoing Activity Signal");
	outActivityTrigger->hideInEditor = true;
	outActivityTrigger->isControllableFeedbackOnly = true;

	
	moduleParams.saveAndLoadRecursiveData = true;
	addChildControllableContainer(&moduleParams);
	
	addChildControllableContainer(&valuesCC);
	valuesCC.includeTriggersInSaveLoad = true;


	commandTester.userCanRemove = false;
	commandTester.canBeDisabled = false;
	commandTester.lockedModule = this;

	controllableContainers.move(controllableContainers.indexOf(scriptManager.get()), controllableContainers.size() - 1);

	addChildControllableContainer(&commandTester);
}

Module::~Module()
{

}

void Module::setupIOConfiguration(bool _hasInput, bool _hasOutput)
{
	if (_hasInput != hasInput) hasInput = _hasInput;
	if (_hasOutput != hasOutput) hasOutput = _hasOutput;
	
	valuesCC.hideInEditor = !hasInput;
	commandTester.hideInEditor = !hasOutput;
	moduleListeners.call(&ModuleListener::moduleIOConfigurationChanged);
}

String Module::getHelpID()
{
	return getTypeString();
}

Array<WeakReference<Controllable>> Module::getValueControllables()
{
	return valuesCC.getAllControllables();
}

void Module::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (cc == &valuesCC)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		scriptManager->callFunctionOnAllItems("moduleValueParamChanged", args);
	}
}

var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", moduleParams.getJSONData());
	return data;
}

void Module::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	moduleParams.loadJSONData(data.getProperty("params", var()));
}

void Module::setupModuleFromJSONData(var data)
{
	customType = data.getProperty("name","");
	setNiceName(data.getProperty("name",""));

	if (data.getDynamicObject()->hasProperty("defaults"))
	{
		var defaultsData = data.getProperty("defaults", var());
		NamedValueSet valueProps = defaultsData.getDynamicObject()->getProperties();
		for (auto &p : valueProps)
		{
			Parameter * c = static_cast<Parameter *>(moduleParams.getControllableByName(p.name.toString()));
			if (c == nullptr) continue;
			c->setValue(p.value);
		}
	}

	if (data.getDynamicObject()->hasProperty("values"))
	{
		var valuesData = data.getProperty("values", var());
		NamedValueSet valueProps = valuesData.getDynamicObject()->getProperties();
		for (auto &p : valueProps)
		{
			Controllable * c = getControllableForJSONDefinition(p.name.toString(), p.value);
			if (c == nullptr) continue;
			valuesCC.addControllable(c);
		}
	}

	if (data.getDynamicObject()->hasProperty("parameters"))
	{
		var valuesData = data.getProperty("parameters", var());
		NamedValueSet valueProps = valuesData.getDynamicObject()->getProperties();
		for (auto &p : valueProps)
		{
			Controllable * c = getControllableForJSONDefinition(p.name.toString(), p.value);
			if (c == nullptr) continue;
			addControllable(c);
		}
	}

	Array<var> * scriptData = data.getProperty("scripts", var()).getArray();
	for (auto &s : *scriptData)
	{
		Script * script = scriptManager->addItem();
		//DBG("Set script path : " << data.getProperty("modulePath", "").toString() << "/" << s.toString());
		script->filePath->setValue(data.getProperty("modulePath", "").toString() + "/"+s.toString());
	}
}

Controllable * Module::getControllableForJSONDefinition(const String &name, var def)
{
	String valueType = def.getProperty("type", "").toString();
	Controllable * c = ControllableFactory::createControllable(valueType);
	if (c == nullptr) return nullptr;

	c->setNiceName(name);
	if (c->type != Controllable::TRIGGER)
	{
		if (def.getDynamicObject()->hasProperty("min") && def.getDynamicObject()->hasProperty("max"))
		{
			((Parameter *)c)->setRange(def.getProperty("min", 0), def.getProperty("max", 1));
		}
		if (def.getDynamicObject()->hasProperty("default"))
		{
			((Parameter *)c)->setValue(def.getProperty("default", 0));
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
