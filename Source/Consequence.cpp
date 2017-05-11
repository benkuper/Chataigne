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
	
}

Consequence::~Consequence()
{

}

void Consequence::triggerCommand()
{
	if (!enabled->boolValue()) return;
	BaseCommandHandler::triggerCommand();
}

/*
InspectableEditor * Consequence::getEditor(bool isRoot)
{
	return new ConsequenceEditor(this, isRoot);
}
*/
