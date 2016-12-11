/*
  ==============================================================================

    Automation.cpp
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#include "Automation.h"

Automation::Automation() :
	BaseManager("Automation"),
	valueMax(1),
	positionMax(1)
{
	
}

Automation::~Automation()
{
}

AutomationKey * Automation::addItem(const float position, const float value)
{
	AutomationKey * k = BaseManager::addItem();
	k->position->setValue(position);
	k->value->setValue(value);
	return k;
}
