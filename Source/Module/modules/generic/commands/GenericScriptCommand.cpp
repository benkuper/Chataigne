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
	BaseCommand(_module, context, params)
{
	addChildControllableContainer(&script);

	if(commandScriptTemplate.isEmpty()) commandScriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic", "command"));
	script.scriptTemplate = &commandScriptTemplate;
}

GenericScriptCommand::~GenericScriptCommand()
{
}

void GenericScriptCommand::setValue(var value)
{
	BaseCommand::setValue(value);
	Array<var> values;
	values.add(value);
	script.callFunction(setValueId, values);
}

void GenericScriptCommand::triggerInternal()
{
	script.callFunction(triggerId, Array<var>());
}

BaseCommand * GenericScriptCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new GenericScriptCommand((ChataigneGenericModule *)module, context, params);

}

