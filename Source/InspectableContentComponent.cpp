/*
  ==============================================================================

    InspectableContentComponent.cpp
    Created: 9 May 2016 6:51:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "InspectableContentComponent.h"
#include "Style.h"

InspectableContentComponent::InspectableContentComponent(Inspectable * inspectable) :
	InspectableContent(inspectable),
	repaintOnSelectionChanged(true),
	bringToFrontOnSelect(true),
	autoDrawHighlightWhenSelected(true),
	rounderCornerSize(4),
	highlightColor(HIGHLIGHT_COLOR),
	autoSelectWithChildRespect(true)
{
}

InspectableContentComponent::~InspectableContentComponent()
{
}

void InspectableContentComponent::mouseDown(const MouseEvent & e)
{
	if (autoSelectWithChildRespect)
	{
		Component * c = e.eventComponent;

		bool foundAChildComponent = false;
		while (c != this)
		{
			InspectableContentComponent * ie = dynamic_cast<InspectableContentComponent *>(c);
			if (ie != nullptr)
			{
				foundAChildComponent = true;
				break;
			}
			c = c->getParentComponent();
		}

		if (!foundAChildComponent)
		{
			inspectable->selectThis();
		}
	}
}

void InspectableContentComponent::paintOverChildren(Graphics & g)
{
	if (autoDrawHighlightWhenSelected && inspectable->isSelected)
	{
		g.setColour(highlightColor);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), rounderCornerSize, 2);
	}
}

void InspectableContentComponent::inspectableSelectionChanged(Inspectable *)
{
	if (bringToFrontOnSelect) toFront(true);
	if (repaintOnSelectionChanged) repaint();
}
