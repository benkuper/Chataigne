/*
  ==============================================================================

    InputSystemManager.cpp
    Created: 17 Jun 2018 5:57:00pm
    Author:  Ben

  ==============================================================================
*/

#include "InputSystemManager.h"

#include "InputDeviceHelpers.h"

juce_ImplementSingleton(InputSystemManager)

InputSystemManager::InputSystemManager() :
	Thread("ISM"),
	inputQueuedNotifier(10)
{

	isInit = SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) == 0;

	if (!isInit)
	{
		LOGERROR("Could not init Input System Manager : " << SDL_GetError());
		return;
	}

	lastCheckTime = 0;
	startThread();
}

InputSystemManager::~InputSystemManager()
{
	signalThreadShouldExit();
	waitForThreadToExit(1000);
	SDL_Quit();
}

Gamepad * InputSystemManager::addGamepad(SDL_GameController * controller)
{
	Gamepad * g = new Gamepad(controller);
	gamepads.add(g); 
	LOG("Gamepad added : " << String(SDL_GameControllerName(g->gamepad)));
	inputListeners.call(&InputManagerListener::gamepadAdded, g);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::GAMEPAD_ADDED, g));
	return g;
}
void InputSystemManager::removeGamepad(Gamepad * g)
{
	if (!gamepads.contains(g)) return;
	gamepads.removeObject(g, false);
	LOG("Gamepad removed : " << String(SDL_GameControllerName(g->gamepad)));
	inputListeners.call(&InputManagerListener::gamepadRemoved, g);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::GAMEPAD_REMOVED, g));
	SDL_GameControllerClose(g->gamepad);
	delete g;

}
Joystick * InputSystemManager::addJoystick(SDL_Joystick * sdlJ)
{
	Joystick * j = new Joystick(sdlJ);
	joysticks.add(j);
	LOG("Joystick added : " << String(SDL_JoystickName(j->joystick)));
	inputListeners.call(&InputManagerListener::joystickAdded, j);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::JOYSTICK_ADDED, j));
	return j;
}
void InputSystemManager::removeJoystick(Joystick * j)
{
	if (!joysticks.contains(j)) return;
	joysticks.removeObject(j, false);
	LOG("Joystick removed : " << String(SDL_JoystickName(j->joystick)));
	inputListeners.call(&InputManagerListener::joystickRemoved, j);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::JOYSTICK_REMOVED, j));
	SDL_JoystickClose(j->joystick);
	delete j;
}

Joystick * InputSystemManager::getJoystickForSDL(SDL_Joystick * tj)
{
	joysticks.getLock().enter();
	for (auto &j : joysticks) if (j->joystick == tj) return j;
	joysticks.getLock().exit();
	return nullptr;
}
Gamepad * InputSystemManager::getGamepadForSDL(SDL_GameController * tg)
{
	gamepads.getLock().enter();
	for (auto &g : gamepads) if (g->gamepad == tg) return g;
	gamepads.getLock().exit();
	return nullptr;
}

Joystick * InputSystemManager::getJoystickForID(SDL_JoystickGUID id)
{
	joysticks.getLock().enter();
	for (auto &j : joysticks)
	{
		SDL_JoystickGUID guid = SDL_JoystickGetGUID(j->joystick);
		bool isTheSame = true; 
		for (int i = 0; i < 16; ++i) if (guid.data[i] != id.data[i]) isTheSame = false;;
		if(isTheSame) return j;
	}
	joysticks.getLock().exit();
	return nullptr;
}

Gamepad * InputSystemManager::getGamepadForID(SDL_JoystickGUID id)
{
	gamepads.getLock().enter();
	for (auto &g : gamepads)
	{
		SDL_JoystickGUID guid = SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(g->gamepad));
		bool isTheSame = true;
		for (int i = 0; i < 16; ++i) if (guid.data[i] != id.data[i]) isTheSame = false;
		if(isTheSame) return g;
	}

	gamepads.getLock().exit();
	return nullptr;
}

Joystick * InputSystemManager::getJoystickForName(String name)
{
	joysticks.getLock().enter();
	for (auto &j : joysticks) if (String(SDL_JoystickName(j->joystick)) == name) return j;
	joysticks.getLock().exit();
	return nullptr;
}

Gamepad * InputSystemManager::getGamepadForName(String name)
{
	gamepads.getLock().enter();
	for (auto &g : gamepads) if (String(SDL_GameControllerName(g->gamepad)) == name) return g;
	gamepads.getLock().exit();
	return nullptr;
}


void InputSystemManager::run()
{
	while (!threadShouldExit())
	{
		if (Time::getApproximateMillisecondCounter() > lastCheckTime + checkDeviceTime)
		{
			int numDevices = SDL_NumJoysticks();
			
			joysticks.getLock().enter();
			gamepads.getLock().enter();

			for (int i = 0; i < numDevices; ++i)
			{
				if (SDL_IsGameController(i))
				{
					SDL_GameController * g = SDL_GameControllerOpen(i);
					if (g == NULL)
					{
						LOG("Unable to open gamepad : " << SDL_GetError());
						continue;
					}
					Gamepad * gp = getGamepadForSDL(g);
					if (gp == nullptr) addGamepad(g);
				} else
				{
					SDL_Joystick * j = SDL_JoystickOpen(i);
					if (j == NULL)
					{
						LOG("Unable to open joystick : " << SDL_GetError());
						continue;
					}
					Joystick * jj = getJoystickForSDL(j);
					if (jj == nullptr) addJoystick(j);
				}
			}

			Array<Joystick*> joysticksToRemove;
			for (auto &j : joysticks)
			{
				//check removed devices
				
				if (!SDL_JoystickGetAttached(j->joystick)) joysticksToRemove.add(j);
			}

			for (auto& j : joysticksToRemove) removeJoystick(j);
			
			Array<Gamepad*> gamepadsToRemove;
			for (auto &g : gamepads)
			{
				//check removed devices
				if (!SDL_GameControllerGetAttached(g->gamepad)) gamepadsToRemove.add(g);
			}
			for (auto& g : gamepadsToRemove) removeGamepad(g);

			
			lastCheckTime = Time::getApproximateMillisecondCounter();
		}

		SDL_JoystickUpdate();
		SDL_GameControllerUpdate();

		for (auto& j : joysticks)
		{
			if (j != nullptr) j->update();
		}

		for (auto& g : gamepads)
		{
			if (g != nullptr) g->update();
		}

		joysticks.getLock().exit();
		gamepads.getLock().exit();


		sleep(10); //100fps
	}
	
}

Joystick::Joystick(SDL_Joystick * joystick) :
	joystick(joystick),
	axesCC("Axes"),
	buttonsCC("Buttons")
{
	int numAxes = SDL_JoystickNumAxes(joystick);
	for (int i = 0; i <  numAxes; ++i)
	{
		FloatParameter * f = axesCC.addFloatParameter("Axis " + String(i + 1), "", 0, -1, 1);
		axisOffset[i] = 0;
		axisDeadZone[i] = 0;
		f->isControllableFeedbackOnly = true;
	}

	int numButtons = SDL_JoystickNumButtons(joystick);
	for (int i = 0; i < numButtons; ++i)
	{
		BoolParameter * b = buttonsCC.addBoolParameter("Button " + String(i + 1), "", false);
		b->isControllableFeedbackOnly = true;
	}
}

Joystick::~Joystick()
{
	masterReference.clear();
}

void Joystick::update()
{
	int numAxes = SDL_JoystickNumAxes(joystick);
	if (axesCC.controllables.size() == numAxes)
	{
		GenericScopedLock lock(axesCC.controllables.getLock());
		for (int i = 0; i < numAxes; ++i)
		{
			float axisValue = jmap<float>((float)SDL_JoystickGetAxis(joystick, i), INT16_MIN, INT16_MAX, -1, 1) + axisOffset[i];
			if (fabs(axisValue) < axisDeadZone[i]) axisValue = 0;
			else
			{
				if (axisValue > 0) axisValue = jmap<float>(axisValue, axisDeadZone[i], 1 + axisOffset[i], 0, 1);
				else axisValue = jmap<float>(axisValue, -1 + axisOffset[i], -axisDeadZone[i], -1, 0);
			}
			((FloatParameter*)axesCC.controllables[i])->setValue(axisValue);
		}
	}

	int numButtons = SDL_JoystickNumButtons(joystick);
	if (buttonsCC.controllables.size() == numButtons)
	{
		GenericScopedLock lock(buttonsCC.controllables.getLock());
		for (int i = 0; i < numButtons; ++i)
		{
			((BoolParameter*)buttonsCC.controllables[i])->setValue(SDL_JoystickGetButton(joystick, i) > 0);
		}
	}
	

}

Gamepad::Gamepad(SDL_GameController * gamepad) :
	gamepad(gamepad),
	axesCC("Axes"),
	buttonsCC("Buttons")
{
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
	{
		SDL_GameControllerAxis a = (SDL_GameControllerAxis)i;

		FloatParameter * f = axesCC.addFloatParameter(SDL_GameControllerGetStringForAxis(a), "", 0, -1, 1);
		f->isControllableFeedbackOnly = true;
		axisOffset[i] = 0;
		axisDeadZone[i] = 0;
	}

	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		SDL_GameControllerButton b = (SDL_GameControllerButton)i;
		BoolParameter * bp = buttonsCC.addBoolParameter(SDL_GameControllerGetStringForButton(b), "", false);
		bp->isControllableFeedbackOnly = true;
	}
}

Gamepad::~Gamepad()
{
	masterReference.clear();
}

void Gamepad::update()
{
	if (axesCC.controllables.size() == SDL_CONTROLLER_AXIS_MAX)
	{
		ScopedLock(axesCC.controllables.getLock());
		for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
		{
			float axisValue = jmap<float>((float)SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis)i), INT16_MIN, INT16_MAX, -1, 1) + axisOffset[i];
			if (fabs(axisValue) < axisDeadZone[i]) axisValue = 0;
			else
			{
				if (axisValue > 0) axisValue = jmap<float>(axisValue, axisDeadZone[i], 1 + axisOffset[i], 0, 1);
				else axisValue = jmap<float>(axisValue, -1 + axisOffset[i], -axisDeadZone[i], -1, 0);
			}
			((FloatParameter*)axesCC.controllables[i])->setValue(axisValue);
		}
	}

	if (buttonsCC.controllables.size() == SDL_CONTROLLER_BUTTON_MAX)
	{
		ScopedLock(buttonsCC.controllables.getLock());
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		{
			((BoolParameter*)buttonsCC.controllables[i])->setValue(SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton)i) > 0);
		}

	}
}


//JOYSTICK PARAMETER

JoystickParameter::JoystickParameter(const String &name, const String &description) :
	Parameter(Controllable::CUSTOM, name, description, var(), var(), var()),
	joystick(nullptr)
{
	for (int i = 0; i < 16; i++)
	{
		value.append(0);
		defaultValue.append(0);
	}
}

JoystickParameter::~JoystickParameter()
{
}

void JoystickParameter::setJoystick(Joystick * j)
{
	if (j == joystick) return;
	if (joystick != nullptr && !joystick.wasObjectDeleted())
	{
		//joystick->removeJoystickListener(this);
	}

	joystick = j;

	if (joystick != nullptr && !joystick.wasObjectDeleted())
	{
		//joystick->addJoystickListener(this);
		var val;
		ghostID = SDL_JoystickGetGUID(joystick->joystick);
		ghostName = SDL_JoystickName(joystick->joystick);
		for (int i = 0; i < 16; ++i) val.append((int)ghostID.data[i]);
		setValue(val, false, true); //Can be the same name, force here

	} else
	{
		var val;
		for (int i = 0; i < 16; ++i) val.append(0);
		setValue(val, false, true);
	}
}

var JoystickParameter::getJSONDataInternal()
{
	var data = Parameter::getJSONDataInternal();
	data.getDynamicObject()->setProperty("deviceName", ghostName);
	return data;
}

void JoystickParameter::loadJSONDataInternal(var data)
{
	if (data.isArray())
	{
		for (int i = 0; i < 16; ++i) ghostID.data[i] = (uint8_t)(int)data[i];
		setJoystick(InputSystemManager::getInstance()->getJoystickForID(ghostID));
	}

	if (joystick == nullptr)
	{
		ghostName = data.getProperty("deviceName", "");
		setJoystick(InputSystemManager::getInstance()->getJoystickForName(ghostName));
	}
}

ControllableUI * JoystickParameter::createDefaultUI()
{
	return new JoystickParameterUI(this);
}





//GAMEPAD PARAMETER

GamepadParameter::GamepadParameter(const String &name, const String &description) :
	Parameter(Controllable::CUSTOM, name, description, var(), var(), var()),
	gamepad(nullptr)
{
	for (int i = 0; i < 16; i++)
	{
		value.append(0);
		defaultValue.append(0);
	}
}

GamepadParameter::~GamepadParameter()
{
}

void GamepadParameter::setGamepad(Gamepad * j)
{
	if (j == gamepad) return;
	if (gamepad != nullptr && !gamepad.wasObjectDeleted())
	{
		//gamepad->removeGamepadListener(this);
	}

	gamepad = j;

	if (gamepad != nullptr && !gamepad.wasObjectDeleted())
	{
		
		//gamepad->addGamepadListener(this);
		var val;
		ghostID = SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(gamepad->gamepad));
		ghostName = SDL_GameControllerName(gamepad->gamepad);
		for (int i = 0; i < 16; ++i) val.append((int)ghostID.data[i]);
		setValue(val, false, true);

	} else
	{
		var val;
		for (int i = 0; i < 16; ++i) val.append(0);
		setValue(val,false,true);
	}
}

var GamepadParameter::getJSONDataInternal()
{
	var data = Parameter::getJSONDataInternal();
	data.getDynamicObject()->setProperty("deviceName", ghostName);
	return data;
}

void GamepadParameter::loadJSONDataInternal(var data)
{
	if (data.isArray())
	{
		for (int i = 0; i < 16; ++i) ghostID.data[i] = (uint8_t)(int)data[i];
		setGamepad(InputSystemManager::getInstance()->getGamepadForID(ghostID));
	}

	if (gamepad == nullptr)
	{
		ghostName = data.getProperty("deviceName", "");
		setGamepad(InputSystemManager::getInstance()->getGamepadForName(ghostName));
	}
}

ControllableUI * GamepadParameter::createDefaultUI()
{
	return new GamepadParameterUI(this);
}
