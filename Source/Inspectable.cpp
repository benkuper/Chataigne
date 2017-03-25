/*
  ==============================================================================

    Inspectable.cpp
    Created: 30 Oct 2016 9:02:24am
    Author:  bkupe

  ==============================================================================
*/

#include "Inspectable.h"
#include "InspectableSelectionManager.h"

Inspectable::Inspectable(const String & _inspectableType) :
	inspectableType(_inspectableType),
	isSelected(false),
	isSelectable(true),
	showInspectorOnSelect(true),
	targetInspector(nullptr) //default nullptr will target main inspector
{
}

Inspectable::~Inspectable()
{
	listeners.call(&InspectableListener::inspectableDestroyed, this);
	masterReference.clear();
}


void Inspectable::selectThis()
{
	if (InspectableSelectionManager::getInstanceWithoutCreating() == nullptr) return;
	InspectableSelectionManager::getInstance()->selectInspectable(this);
}

void Inspectable::setSelected(bool value)
{
	if (!isSelectable) return; 
	if (value == isSelected) return;

	isSelected = value;
	isPreselected = false;

	setSelectedInternal(value);

	listeners.call(&InspectableListener::inspectableSelectionChanged, this);
}

void Inspectable::setPreselected(bool value)
{
	if (!isSelectable) return;
	if (value == isPreselected) return;

	isPreselected = value;

	listeners.call(&InspectableListener::inspectablePreselectionChanged, this);
}

void Inspectable::setSelectedInternal(bool)
{
	//to be overriden
}
