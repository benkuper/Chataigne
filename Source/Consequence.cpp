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
		DBG("consequence trigger");
		if (command != nullptr) command->trigger();
	}
}

var Consequence::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (command != nullptr)
	{
		data.getDynamicObject()->setProperty("commandModule", command->container->getControlAddress());
		data.getDynamicObject()->setProperty("commandPath", commandDefinition->menuPath);
		data.getDynamicObject()->setProperty("commandType", commandDefinition->commandType);
	}
	return data;
}

void Consequence::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("commandModule"))
	{
		Module * m = (Module *)Engine::getInstance()->getControllableContainerForAddress(data.getProperty("commandModule", ""));
		if (m != nullptr)
		{
			
			String menuPath = data.getProperty("commandPath", "");
			String commandType = data.getProperty("commandType", "");
			setCommand(m->defManager.getCommandDefinitionFor(menuPath, commandType));
		} else
		{
			DBG("Output not found : " << data.getProperty("commandModule", "").toString());
		}
	}

}
