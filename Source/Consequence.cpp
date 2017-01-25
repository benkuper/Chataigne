/*
  ==============================================================================

    Consequence.cpp
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#include "Consequence.h"
#include "ConsequenceEditor.h"

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

InspectableEditor * Consequence::getEditor(bool isRoot)
{
	return new ConsequenceEditor(this, isRoot);
}
