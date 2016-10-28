/*
  ==============================================================================

    ShapeShifter.h
    Created: 2 May 2016 6:33:16pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTER_H_INCLUDED
#define SHAPESHIFTER_H_INCLUDED

#include "JuceHeader.h"//keep

class ShapeShifterContainer;

class ShapeShifter : public Component
{
public :
	enum Type {PANEL, CONTAINER};

	ShapeShifter(Type _type);
	virtual ~ShapeShifter();

	ShapeShifterContainer * parentContainer;

	Type shifterType;

	int preferredWidth;
	int preferredHeight;
	void setPreferredWidth(int newWidth);
	void setPreferredHeight(int newHeight);

	virtual int getPreferredWidth();
	virtual int getPreferredHeight();

	bool isDetached();
	void setParentContainer(ShapeShifterContainer * _parent);

	virtual bool isFlexible();

	virtual var getCurrentLayout();
	virtual void loadLayout(var layout);
	virtual void loadLayoutInternal(var /*layout*/) {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifter)
};


#endif  // SHAPESHIFTER_H_INCLUDED
