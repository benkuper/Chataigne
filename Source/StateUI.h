/*
  ==============================================================================

    StateUI.h
    Created: 28 Oct 2016 8:19:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEUI_H_INCLUDED
#define STATEUI_H_INCLUDED


#include "BaseItemUI.h"
#include "State.h"

class StateUI :
	public BaseItemUI<State>
{
public:
	StateUI(State * output);
	virtual ~StateUI();

	JUCE_DECLARE_NON_COPYABLE(StateUI)
};



#endif  // STATEUI_H_INCLUDED
