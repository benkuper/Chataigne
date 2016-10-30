/*
  ==============================================================================

    InputManagerUI.h
    Created: 28 Oct 2016 8:22:22pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INPUTMANAGERUI_H_INCLUDED
#define INPUTMANAGERUI_H_INCLUDED

#include "BaseManagerShapeShifterUI.h"
#include "InputUI.h"
#include "InputManager.h"

class InputManagerUI :
	public BaseManagerShapeShifterUI<InputManager,Input, InputUI>
{
public:

	InputManagerUI(InputManager *_manager);
	~InputManagerUI();

	void mouseDown(const MouseEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputManagerUI)
};


#endif  // INPUTMANAGERUI_H_INCLUDED
