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

class ShapeShifterContent
{
public:
	ShapeShifterContent(Component * contentComponent, const String &_contentName);
	virtual ~ShapeShifterContent();

	Component * contentComponent;
	String contentName;

	bool contentIsFlexible;

	bool contentIsShown;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterContent)
};

//Helper class if child class doesn't need to inherit a subclass of Component
class ShapeShifterContentComponent :
	public Component,
	public ShapeShifterContent
{
public:
	ShapeShifterContentComponent(const String &contentName) :
		ShapeShifterContent(this, contentName)
	{

	}
};

#endif  // SHAPESHIFTERCONTENT_H_INCLUDED
