/*
  ==============================================================================

    InputManagerUI.cpp
    Created: 28 Oct 2016 8:22:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputManagerUI.h"


InputManagerUI::InputManagerUI(InputManager * _manager) :
	BaseManagerShapeShifterUI<InputManager,Input,InputUI>("Inputs", _manager)
{
	addItemText = "Add Input";
}

InputManagerUI::~InputManagerUI()
{
}
