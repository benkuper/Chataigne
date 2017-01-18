/*
  ==============================================================================

    Inspectable.cpp
    Created: 30 Oct 2016 9:02:24am
    Author:  bkupe

  ==============================================================================
*/

#include "Inspectable.h"
#include "Inspector.h"
#include "ShapeShifterManager.h"

Inspectable::Inspectable(const String & _inspectableType) :
	inspectableType(_inspectableType),
	isSelected(false),
	isSelectable(true)
{
}

Inspectable::~Inspectable()
{
	listeners.call(&InspectableListener::inspectableDestroyed, this);
	masterReference.clear();
}


void Inspectable::selectThis()
{
	if (Inspector::getInstanceWithoutCreating() == nullptr)
	{
		DBG("Create an Inspector !");
		ShapeShifterManager::getInstance()->showPanelWindowForContent(PanelName::InspectorPanel);
	}

	Inspector::getInstance()->setCurrentInspectable(this);
}

void Inspectable::setSelected(bool value)
{
	if (!isSelectable) return; 
	if (value == isSelected) return;

	isSelected = value;

	setSelectedInternal(value);

	listeners.call(&InspectableListener::inspectableSelectionChanged, this);
}

void Inspectable::setSelectedInternal(bool)
{
	//to be overriden
}
