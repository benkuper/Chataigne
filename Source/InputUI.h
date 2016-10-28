/*
  ==============================================================================

    InputUI.h
    Created: 28 Oct 2016 8:22:16pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INPUTUI_H_INCLUDED
#define INPUTUI_H_INCLUDED

#include "BaseItemUI.h"
#include "Input.h"

class InputUI :
	public BaseItemUI<Input>
{
public :
	InputUI(Input *);
	virtual ~InputUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputUI)
};



#endif  // INPUTUI_H_INCLUDED
