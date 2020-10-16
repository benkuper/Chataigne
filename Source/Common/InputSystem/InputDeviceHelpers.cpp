/*
  ==============================================================================

    InputDeviceHelpers.cpp
    Created: 17 Jun 2018 8:45:30pm
    Author:  Ben

  ==============================================================================
*/

#include "InputDeviceHelpers.h"


JoystickParameterUI::JoystickParameterUI(JoystickParameter * p) :
	ParameterUI(p),
	joystickParam(p)
{
	chooser.setTextWhenNoChoicesAvailable("No joystick connected");
	chooser.setTextWhenNothingSelected("Select a joystick");
	chooser.addListener(this);
	addAndMakeVisible(&chooser);
	InputSystemManager::getInstance()->addAsyncInputListener(this);

	rebuild();
}

JoystickParameterUI::~JoystickParameterUI()
{
	if(InputSystemManager::getInstanceWithoutCreating() != nullptr) InputSystemManager::getInstance()->removeAsyncInputListener(this);
}

WeakReference<Joystick> JoystickParameterUI::getJoystick()
{
	return chooser.getSelectedId() > 0 ? joysticks[chooser.getSelectedId() - 1] : nullptr;
}

void JoystickParameterUI::rebuild()
{
	WeakReference<Joystick> selectedJoystick = getJoystick();

	chooser.clear(dontSendNotification);
	chooser.setTextWhenNothingSelected(joystickParam->ghostName.isNotEmpty() ? joystickParam->ghostName + " disconnected" : "Select a joystick");
	chooser.addItem("Not connected", -2);

	if (InputSystemManager::getInstance()->joysticks.size() == 0) return;

	int id = 1;
	int idToSelect = -1;
	for (auto &j : InputSystemManager::getInstance()->joysticks)
	{
		joysticks.add(j);
		chooser.addItem(SDL_JoystickName(j->joystick), id);
		if (j == selectedJoystick || j == joystickParam->joystick) idToSelect = id;
		id++;
	}
	
	chooser.setSelectedId(idToSelect, dontSendNotification);
}

void JoystickParameterUI::resized()
{
	chooser.setBounds(getLocalBounds());
}

void JoystickParameterUI::newMessage(const InputSystemManager::InputSystemEvent & e)
{
	switch(e.type)
	{
	case InputSystemManager::InputSystemEvent::JOYSTICK_ADDED:
	case InputSystemManager::InputSystemEvent::JOYSTICK_REMOVED:
		rebuild();
		break;

	default:
		break;
	}
}


void JoystickParameterUI::comboBoxChanged(ComboBox *)
{
	if (!parameter.wasObjectDeleted())
	{
		if (chooser.getSelectedId() == -2)
		{
			joystickParam->ghostID = SDL_JoystickGUID();
			joystickParam->ghostName = "";
		}
		if (!parameter.wasObjectDeleted()) joystickParam->setJoystick(getJoystick());
	}
}


GamepadParameterUI::GamepadParameterUI(GamepadParameter * p) :
	ParameterUI(p),
	gamepadParam(p)
{
	chooser.setTextWhenNoChoicesAvailable("No gamepad connected");
	chooser.setTextWhenNothingSelected("Select a gamepad");
	chooser.addListener(this);
	addAndMakeVisible(&chooser);

	InputSystemManager::getInstance()->addAsyncInputListener(this);

	rebuild();
}

GamepadParameterUI::~GamepadParameterUI()
{
	if (InputSystemManager::getInstanceWithoutCreating() != nullptr) InputSystemManager::getInstance()->removeAsyncInputListener(this);
}

WeakReference<Gamepad> GamepadParameterUI::getGamepad()
{
	return chooser.getSelectedId() > 0 ? gamepads[chooser.getSelectedId() - 1] : nullptr;
}

void GamepadParameterUI::rebuild()
{
	WeakReference<Gamepad> selectedGamepad = getGamepad();

	chooser.clear(dontSendNotification);
	chooser.setTextWhenNothingSelected(gamepadParam->ghostName.isNotEmpty()?gamepadParam->ghostName+" disconnected":"Select a gamepad");
	chooser.addItem("Not connected", -2);

	if (InputSystemManager::getInstance()->gamepads.size() == 0) return;

	int id = 1;
	int idToSelect = -1;
	for (auto &g : InputSystemManager::getInstance()->gamepads)
	{
		gamepads.add(g);
		String gName = SDL_GameControllerName(g->gamepad);
		chooser.addItem(gName, id);
		if (g == selectedGamepad || g == gamepadParam->gamepad) idToSelect = id;
		id++;
	}

	chooser.setSelectedId(idToSelect,dontSendNotification);
}

void GamepadParameterUI::resized()
{
	chooser.setBounds(getLocalBounds());
}

void GamepadParameterUI::valueChanged(const var & v)
{
	ParameterUI::valueChanged(v);
	chooser.setSelectedId(gamepads.indexOf(gamepadParam->gamepad) + 1);
}

void GamepadParameterUI::newMessage(const InputSystemManager::InputSystemEvent & e)
{
	switch (e.type)
	{
	case InputSystemManager::InputSystemEvent::GAMEPAD_ADDED:
	case InputSystemManager::InputSystemEvent::GAMEPAD_REMOVED:
		rebuild();
		break;

	default:
		break;
	}
}

void GamepadParameterUI::comboBoxChanged(ComboBox * c)
{
	if (!parameter.wasObjectDeleted())
	{
		if (chooser.getSelectedId() == -2)
		{
			gamepadParam->ghostID = SDL_JoystickGUID();
			gamepadParam->ghostName = "";
		}
		gamepadParam->setGamepad(getGamepad());
	}
}
