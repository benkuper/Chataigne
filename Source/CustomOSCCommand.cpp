/*
  ==============================================================================

    CustomOSCCommand.cpp
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCCommand.h"
#include "CustomOSCCommandEditor.h"

CustomOSCCommand::CustomOSCCommand(CustomOSCModule * module, CommandContext context, var params) :
	OSCCommand(module, context, params)
{
}

CustomOSCCommand::~CustomOSCCommand()
{
}


void CustomOSCCommand::addIntArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new IntParameter("#" + id, "Argument #" + id + ", type int", 0, -1000, 1000);
	addArgument(p);
}

void CustomOSCCommand::addFloatArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new FloatParameter("#" + id, "Argument #" + id + ", type int", 0, 0, 1);
	addArgument(p);
}

void CustomOSCCommand::addStringArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new StringParameter("#" + id, "Argument #" + id + ", type int", "myString");
	addArgument(p);
}

void CustomOSCCommand::addArgument(Parameter * p)
{
	p->saveValueOnly = false;
	p->isRemovableByUser = true; 
	argumentsContainer.addControllable(p);
}

void CustomOSCCommand::removeLastArgument()
{
	if (argumentsContainer.controllables.size() == 0) return;
	argumentsContainer.removeControllable(argumentsContainer.controllables[argumentsContainer.controllables.size() - 1]);
}

var CustomOSCCommand::getJSONData()
{
	var data = OSCCommand::getJSONData();
	data.getDynamicObject()->setProperty("arguments", argumentsContainer.getJSONData());
	return data;
}

void CustomOSCCommand::loadJSONDataInternal(var data)
{
	OSCCommand::loadJSONDataInternal(data);
	DBG("here custom OSCCommand loadJSON Data internal");
	argumentsContainer.loadJSONData(data.getProperty("arguments", var()), true);
}

InspectableEditor * CustomOSCCommand::getEditor(bool)
{
	return new CustomOSCCommandEditor(this, false);
}
