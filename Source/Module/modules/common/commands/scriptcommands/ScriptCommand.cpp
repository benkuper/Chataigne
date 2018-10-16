/*
  ==============================================================================

    ScriptCommand.cpp
    Created: 1 Aug 2018 5:47:34pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptCommand.h"

ScriptCommand::ScriptCommand(Module * module, CommandContext context, var data) :
	BaseCommand(module, context, data)
{
	//load params here
	callback = data.getProperty("callback", "defaultCallback").toString();

	var pData = data.getProperty("params", var());
	if (pData.isObject())
	{
		NamedValueSet paramsData = pData.getDynamicObject()->getProperties();
	}
	
}

ScriptCommand::~ScriptCommand()
{
}
