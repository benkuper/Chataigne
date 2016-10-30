/*
  ==============================================================================

    InspectableContentComponent.h
    Created: 9 May 2016 6:51:16pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTABLECONTENTCOMPONENT_H_INCLUDED
#define INSPECTABLECONTENTCOMPONENT_H_INCLUDED

#include "InspectableContent.h"

class InspectableContentComponent : 
	public InspectableContent,
	public Component
{
public:
	InspectableContentComponent(Inspectable * inspectable);
	virtual ~InspectableContentComponent();

	bool repaintOnSelectionChanged;

	float rounderCornerSize;
	bool autoDrawHighlightWhenSelected;
	Colour highlightColor;

	bool autoSelectWithChildRespect;
	bool bringToFrontOnSelect;

	void mouseDown(const MouseEvent &e) override;

	virtual void paintOverChildren(Graphics &g) override;

	void inspectableSelectionChanged(Inspectable *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectableContentComponent)
};



#endif  // INSPECTABLECONTENTCOMPONENT_H_INCLUDED
