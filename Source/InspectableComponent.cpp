/*
  ==============================================================================

    InspectableComponent.cpp
    Created: 9 May 2016 6:51:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "InspectableComponent.h"
#include "Inspector.h"
#include "CustomEditor.h"
#include "GenericControllableContainerEditor.h"
#include "MainComponent.h"
#include "ShapeShifterFactory.h"
#include "ShapeShifterManager.h"

InspectableComponent::InspectableComponent(ControllableContainer * relatedContainer, const String &_inspectableType) :
	inspectableType(_inspectableType),
	relatedControllableContainer(relatedContainer),
	recursiveInspectionLevel(0),
	canInspectChildContainersBeyondRecursion(true),
	isSelected(false),
	repaintOnSelectionChanged(true),
	bringToFrontOnSelect(true),
	autoDrawHighlightWhenSelected(true),
	rounderCornerSize(4),
	autoSelectWithChildRespect(true)
{
}

InspectableComponent::~InspectableComponent()
{
	listeners.call(&InspectableListener::inspectableRemoved,this);
}

InspectorEditor * InspectableComponent::getEditor()
{
	return new GenericControllableContainerEditor(this);
}

void InspectableComponent::mouseDown(const MouseEvent & e)
{
	if (autoSelectWithChildRespect)
	{
		Component * c = e.eventComponent;

		bool foundAChildComponent = false;
		while (c != this)
		{
			InspectableComponent * ie = dynamic_cast<InspectableComponent *>(c);
			if (ie != nullptr)
			{
				foundAChildComponent = true;
				break;
			}
			c = c->getParentComponent();
		}

		//DBG(relatedControllableContainer->niceName << " found a child component ? " << String(foundAChildComponent));
		if (!foundAChildComponent) selectThis();
	}
}

void InspectableComponent::paintOverChildren(Graphics & g)
{
	if (autoDrawHighlightWhenSelected && isSelected)
	{
		g.setColour(HIGHLIGHT_COLOR);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), rounderCornerSize, 2);
	}
}

void InspectableComponent::selectThis()
{
	if (Inspector::getInstanceWithoutCreating() == nullptr)
	{
		ShapeShifterManager::getInstance()->showPanelWindowForContent(PanelName::InspectorPanel);
	}
	Inspector::getInstance()->setCurrentComponent(this);
}

void InspectableComponent::setSelected(bool value)
{
	if (value == isSelected) return;
	isSelected = value;

	if (bringToFrontOnSelect) toFront(true);
	if (repaintOnSelectionChanged) repaint();


	setSelectedInternal(value);

	listeners.call(&InspectableListener::inspectableSelectionChanged, this);
}

void InspectableComponent::setSelectedInternal(bool)
{
	//to be overriden
}
