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
	BaseCommandHandler("Consequence")
{
	isSelectable = false;

	trigger = addTrigger("Trigger", "Trigger this consequence");
}

Consequence::~Consequence()
{
}



void Consequence::onContainerTriggerTriggered(Trigger * t)
{
	if (t == trigger)
	{
		if (command != nullptr) command->trigger();
	}
}