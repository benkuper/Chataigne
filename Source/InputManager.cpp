/*
  ==============================================================================

    InputManager.cpp
    Created: 28 Oct 2016 8:02:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputManager.h"

juce_ImplementSingleton(InputManager)

InputManager::InputManager() :
	BaseManager<Input>("Inputs")
{
}

InputManager::~InputManager()
{
}
