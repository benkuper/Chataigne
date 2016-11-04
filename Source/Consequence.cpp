/*
  ==============================================================================

    Consequence.cpp
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#include "Consequence.h"
#include "CommandFactory.h"
#include "Engine.h"

Consequence::Consequence() :
	BaseItem("Consequence")
{
	saveAndLoadRecursiveData = false;
	trigger = addTrigger("Trigger", "Trigger this consequence");
}

Consequence::~Consequence()
{
}


void Consequence::setCommand(CommandDefinition * commandDef)
{
	if (command != nullptr)
	{

	}

	commandDefinition = commandDef;
	if (commandDef != nullptr) command = commandDef->create(CommandContext::ACTION);

	else command = nullptr;

	if (command != nullptr)
	{

	}

	consequenceListeners.call(&ConsequenceListener::consequenceCommandChanged, this);
}

void Consequence::onContainerTriggerTriggered(Trigger * t)
{
	if (t == trigger)
	{
		if (command != nullptr) command->trigger();
	}
}

var Consequence::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (command != nullptr)
	{
		data.getDynamicObject()->setProperty("commandOutput", command->container->getControlAddress());
		data.getDynamicObject()->setProperty("commandPath", commandDefinition->menuPath);
		data.getDynamicObject()->setProperty("commandType", commandDefinition->inputType);
	}
	return data;
}

void Consequence::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("commandOutput"))
	{
		Output * o = (Output *)Engine::getInstance()->getControllableContainerForAddress(data.getProperty("commandOutput", ""));
		if (o != nullptr)
		{
			
			String menuPath = data.getProperty("commandPath", "");
			String inputType = data.getProperty("commandType", "");
			setCommand(o->getCommandDefinitionFor(menuPath, inputType));
		} else
		{
			DBG("Output not found : " << data.getProperty("commandOutput", "").toString());
		}
	}

}
