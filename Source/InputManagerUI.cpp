/*
  ==============================================================================

    InputManagerUI.cpp
    Created: 28 Oct 2016 8:22:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputManagerUI.h"
#include "InputFactory.h"

InputManagerUI::InputManagerUI(InputManager * _manager) :
	BaseManagerShapeShifterUI<InputManager,Input,InputUI>("Inputs", _manager)
{
	useDefaultMenu = false;

}

InputManagerUI::~InputManagerUI()
{
}

void InputManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isRightButtonDown())
	{
		Input * i = InputFactory::getInstance()->showCreateMenu();
		if (i != nullptr) manager->addItem(i);
	}
}
