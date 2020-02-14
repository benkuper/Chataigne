/*
  ==============================================================================

    GenericScriptCommand.cpp
    Created: 6 Apr 2019 1:01:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericScriptCommand.h"
#include "UI/ChataigneAssetManager.h"

String GenericScriptCommand::commandScriptTemplate = "";

GenericScriptCommand::GenericScriptCommand(ChataigneGenericModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	script(this, false)
{

	addChildControllableContainer(&script);

	if(commandScriptTemplate.isEmpty()) commandScriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic", "command"));
	script.scriptTemplate = &commandScriptTemplate;
}

GenericScriptCommand::~GenericScriptCommand()
{
}

void GenericScriptCommand::setValueInternal(var value)
{
	Array<var> values;
	values.add(value);
	script.callFunction(setValueId, values);
}

void GenericScriptCommand::triggerInternal()
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

BaseCommand * GenericScriptCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new GenericScriptCommand((ChataigneGenericModule *)module, context, params);

}

