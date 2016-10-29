/*
  ==============================================================================

    State.h
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED


#include "BaseItem.h"

class State :
	public BaseItem
{
public:
	State();
	virtual ~State();

	//UI
	Point2DParameter * editorPosition;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(State)
};



#endif  // STATE_H_INCLUDED
