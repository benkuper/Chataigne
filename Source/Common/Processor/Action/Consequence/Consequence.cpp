/*
  ==============================================================================

    Consequence.cpp
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#include "Consequence.h"

Consequence::Consequence(IteratorProcessor * iterator) :
	BaseCommandHandler("Consequence",CommandContext::ACTION, nullptr, iterator),
	forceDisabled(false)
{
	
	isSelectable = false;
}

Consequence::~Consequence()
{

}

void Consequence::triggerCommand(int iterationIndex)
{
	if (!enabled->boolValue() || forceDisabled) return;
	BaseCommandHandler::triggerCommand(iterationIndex);
}