/*
  ==============================================================================

    Input.h
    Created: 28 Oct 2016 8:02:24pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "BaseItem.h"

class Input :
	public BaseItem
{
public:
	Input(const String &name = "Input");
	virtual ~Input();

	BoolParameter * logIncomingData; 
	Trigger * activityTrigger;

	ControllableContainer valuesCC;
	Array<WeakReference<Controllable>> getValueControllables();

	virtual var getJSONData() override;

	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Input)
};



#endif  // INPUT_H_INCLUDED
