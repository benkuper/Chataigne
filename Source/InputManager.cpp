/*
  ==============================================================================

    InputManager.cpp
    Created: 28 Oct 2016 8:02:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputManager.h"
#include "InputFactory.h"

juce_ImplementSingleton(InputManager)

InputManager::InputManager() :
	BaseManager<Input>("Inputs")
{
}

InputManager::~InputManager()
{
	InputFactory::deleteInstance();
}

void InputManager::addItemFromData(var data)
{
	String inputType = data.getProperty("type","none");
	if (inputType.isEmpty()) return;
	Input * i = InputFactory::getInstance()->createInput(inputType);
	if(i != nullptr) addItem(i,data);
}
