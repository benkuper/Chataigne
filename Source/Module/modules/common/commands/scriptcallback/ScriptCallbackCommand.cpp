/*
  ==============================================================================

    ScriptCallbackCommand.cpp
    Created: 12 Jul 2019 10:43:37am
    Author:  bkupe

  ==============================================================================
*/

#include "ScriptCallbackCommand.h"

ScriptCallbackCommand::ScriptCallbackCommand(Module* module, CommandContext context, var params) :
	BaseCommand(module, context, params)
{
	moduleMethods = addEnumParameter("Callback", "The function to trigger when the command is triggered");
	
	StringArray avoidMethods{ "exec", "eval", "trace", "charToInt", "parseInt", "typeof", "parseFloat", "Object", "Array", "String", "Math", "JSON", "Integer", "script", "local", "root", "util" };
	StringArray methods;
	for(auto &s : module->scriptManager->items)
	{
		if (s->state != Script::ScriptState::SCRIPT_LOADED) continue;

		const NamedValueSet props = s->scriptEngine->getRootObjectProperties();
		for (auto& sp : props)
		{
			if ((sp.value.isMethod() || sp.value.isUndefined() || sp.value.isObject()) && !avoidMethods.contains(sp.name) && !methods.contains(sp.name))
			{
				methods.add(sp.name.toString());
			}
		}
	}

	for (auto& m : methods)
	{
		moduleMethods->addOption(m, m);
	}

	customValuesManager.reset(new CustomValuesCommandArgumentManager(true, false));
	addChildControllableContainer(customValuesManager.get());
	customValuesManager->addArgumentManagerListener(this);
}

ScriptCallbackCommand::~ScriptCallbackCommand()
{
}


void ScriptCallbackCommand::useForMappingChanged(CustomValuesCommandArgument* )
{
	if (context != MAPPING) return;

	clearTargetMappingParameters();
	int index = 0;
	if (customValuesManager == nullptr) return;

	for (auto& a : customValuesManager->items)
	{
		if (a->useForMapping->boolValue())
		{
			addTargetMappingParameterAt(a->param, index);
			index++;
		}
	}
}

void ScriptCallbackCommand::triggerInternal()
{
	Array<var> args;
	for (auto& i : customValuesManager->items) args.add(i->param->value);
	String mName = moduleMethods->getValueData().toString();

	module->scriptManager->callFunctionOnAllItems(mName, args);
}

var ScriptCallbackCommand::getJSONData()
{
	var data = BaseCommand::getJSONData();
	data.getDynamicObject()->setProperty("customValues", customValuesManager->getJSONData());
	return data;
}

void ScriptCallbackCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("customValues", var()));
}

