/*
  ==============================================================================

    Condition.cpp
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Condition.h"

Condition::Condition() :
	BaseItem("Condition")
{
	isActive = addBoolParameter("Is Active", "Where the condition passed the test or not.", false);
	isActive->isEditable = false;

	isValid = addBoolParameter("Is Valid", "Whether the condition is well parametered and can be processed.", false);
	isValid->isEditable = false;
}

Condition::~Condition()
{
}
