/*
  ==============================================================================

    InputManager.h
    Created: 28 Oct 2016 8:02:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INPUTMANAGER_H_INCLUDED
#define INPUTMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "Input.h"
class InputManager :
	public BaseManager<Input>
{
public:
	juce_DeclareSingleton(InputManager, true)

	InputManager();
	~InputManager();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputManager)

};
#endif  // INPUTMANAGER_H_INCLUDED
