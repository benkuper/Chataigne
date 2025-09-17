/*
  ==============================================================================

    ScriptCallbackCommand.cpp
    Created: 12 Jul 2019 10:43:37am
    Author:  bkupe

  ==============================================================================
*/

ScriptCallbackCommand::ScriptCallbackCommand(Module* module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(module, context, params, multiplex)
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

	setUseCustomValues(true);
}

ScriptCallbackCommand::~ScriptCallbackCommand()
{
}

void ScriptCallbackCommand::triggerInternal(int multiplexIndex)
{
	Array<var> args;
	for (auto& i : customValuesManager->items) args.add(i->getLinkedValue(multiplexIndex));
	String mName = moduleMethods->getValueData().toString();
	if(mName.isNotEmpty()) module->scriptManager->callFunctionOnAllItems(mName, args);
}

var ScriptCallbackCommand::getJSONData(bool includeNonOverriden)
{
	var data = BaseCommand::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("customValues", customValuesManager->getJSONData());
	return data;
}

void ScriptCallbackCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("customValues", var()));
}

