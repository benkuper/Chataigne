/*
  ==============================================================================

    Consequence.cpp
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#include "Consequence.h"
#include "CommandFactory.h"

Consequence::Consequence() :
	BaseItem("Consequence")
{
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
