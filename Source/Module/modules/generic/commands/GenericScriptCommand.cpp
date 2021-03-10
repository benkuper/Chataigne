/*
  ==============================================================================

    GenericScriptCommand.cpp
    Created: 6 Apr 2019 1:01:44pm
    Author:  bkupe

  ==============================================================================
*/

String GenericScriptCommand::commandScriptTemplate = "";

GenericScriptCommand::GenericScriptCommand(ChataigneGenericModule * _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	script(this, false)
{

	addChildControllableContainer(&script);

	if(commandScriptTemplate.isEmpty()) commandScriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic", "command"));
	script.scriptTemplate = &commandScriptTemplate;
}

GenericScriptCommand::~GenericScriptCommand()
{
}

void GenericScriptCommand::setValueInternal(var value, int multiplexIndex)
{
	Array<var> values;
	values.add(value);
	script.callFunction(setValueId, values);
}

void GenericScriptCommand::triggerInternal(int multiplexIndex)
{
	if(context != MAPPING) script.callFunction(triggerId, Array<var>());
}

var GenericScriptCommand::getJSONData()
{
	var data = BaseCommand::getJSONData();
	data.getDynamicObject()->setProperty("script", script.getJSONData());
	return data;
}

void GenericScriptCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	script.loadJSONData(data.getProperty("script", var()));
}

BaseCommand * GenericScriptCommand::create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex)
{
	return new GenericScriptCommand((ChataigneGenericModule *)module, context, params, multiplex);

}

