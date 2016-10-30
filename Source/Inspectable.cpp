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
	recursiveInspectionLevel(0),
	canInspectChildContainersBeyondRecursion(true),
	isSelected(false)
{
}

Inspectable::~Inspectable()
{
	listeners.call(&InspectableListener::inspectableDestroyed, this);
}


void Inspectable::selectThis()
{
	if (Inspector::getInstanceWithoutCreating() == nullptr)
	{
		ShapeShifterManager::getInstance()->showPanelWindowForContent(PanelName::InspectorPanel);
	}

	Inspector::getInstance()->setCurrentInspectable(this);
}

void Inspectable::setSelected(bool value)
{
	if (value == isSelected) return;
	isSelected = value;

	setSelectedInternal(value);

	listeners.call(&InspectableListener::inspectableSelectionChanged, this);
}

void Inspectable::setSelectedInternal(bool)
{
	//to be overriden
}
