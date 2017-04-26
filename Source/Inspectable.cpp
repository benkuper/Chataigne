/*
  ==============================================================================

    Inspectable.cpp
    Created: 30 Oct 2016 9:02:24am
    Author:  bkupe

  ==============================================================================
*/

#include "Inspectable.h"
#include "InspectableSelectionManager.h"

Inspectable::Inspectable() :
selectionManager(nullptr), //default nullptr will target main selectionManager
	isSelected(false),
isSelectable(true),
	showInspectorOnSelect(true)
{
	setSelectionManager(nullptr);
}

Inspectable::~Inspectable()
{
	listeners.call(&InspectableListener::inspectableDestroyed, this);
	masterReference.clear();
}


void Inspectable::selectThis()
{
	if (selectionManager == nullptr) return;
	selectionManager->selectInspectable(this);
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

void Inspectable::setSelectionManager(InspectableSelectionManager * _selectionManager)
{
	if (selectionManager == _selectionManager && selectionManager != nullptr) return;

	if (_selectionManager != nullptr)
	{
		selectionManager = _selectionManager;
	}else if(InspectableSelectionManager::getInstanceWithoutCreating() != nullptr)
	{
		selectionManager = InspectableSelectionManager::getInstance();
	}

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
