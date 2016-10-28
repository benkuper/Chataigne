/*
  ==============================================================================

    ShapeShifterContent.h
    Created: 3 May 2016 10:17:41am
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERCONTENT_H_INCLUDED
#define SHAPESHIFTERCONTENT_H_INCLUDED

#include "JuceHeader.h"//keep
#include "InspectableComponent.h"

class ShapeShifterContent : public InspectableComponent
{
public:
	ShapeShifterContent(const String &_contentName);
	virtual ~ShapeShifterContent();

	String contentName;

	bool contentIsFlexible;

	bool contentIsShown;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterContent)
};


#endif  // SHAPESHIFTERCONTENT_H_INCLUDED
