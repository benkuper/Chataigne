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
#include "ControllableFactory.h"
#include "ScriptManager.h"

Module::Module(const String &name) :
	BaseItem(name,true,true),
	valuesCC("Values"),
	commandTester("Command Tester",CommandContext::ACTION),
	canHandleRouteValues(false)
{
	commandTester.canBeDisabled = false;
	canInspectChildContainers = true;

	addChildControllableContainer(&valuesCC);

	logIncomingData = addBoolParameter("Log Incoming", "Enable / Disable logging of incoming data for this module", false);
	logIncomingData->hideInOutliner = true;
	logIncomingData->isTargettable = false;

	logOutgoingData = addBoolParameter("Log Outgoing", "Enable / Disable logging of outgoing data for this module", false);
	logOutgoingData->hideInOutliner = true;
	logOutgoingData->isTargettable = false;

	inActivityTrigger = addTrigger("IN Activity", "Incoming Activity Signal");
	inActivityTrigger->hideInEditor = true;

	outActivityTrigger = addTrigger("OUT Activity", "Outgoing Activity Signal");
	outActivityTrigger->hideInEditor = true;
}

Module::~Module()
{

}

Array<WeakReference<Controllable>> Module::getValueControllables()
{
	return valuesCC.getAllControllables();
}

void Module::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Array<var> args;
	args.add(c->createScriptObject());
	if(c->type == Controllable::TRIGGER) scriptManager->callFunctionOnAllItems("moduleValueTriggered", args);
	else scriptManager->callFunctionOnAllItems("moduleParamChanged", args);
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
			Parameter * c = static_cast<Parameter *>(getControllableByName(p.name.toString()));
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


var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	//if (sourceDefinitionPath.isNotEmpty()) data.getDynamicObject()->setProperty("customDefinitionPath", sourceDefinitionPath);
	return data;
}

void Module::loadJSONDataInternal(var data)
{
	/*
	if (data.getDynamicObject()->hasProperty("customDefinitionPath"))
	{
		String path = data.getProperty("customDefinitionPath", "").toString();
		var moduleData = JSON::parse(File(path));
		if (moduleData == nullptr)
		{
			DBG("File not found : " << data.getProperty("customDefinitionPath", "").toString());
		}
		else
		{
			moduleData.getDynamicObject()->setProperty("modulePath", path);
		}

		setupModuleFromJSONData(moduleData); 
	}
	*/
	BaseItem::loadJSONDataInternal(data);
}
