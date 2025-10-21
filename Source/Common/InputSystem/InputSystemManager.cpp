/*
  ==============================================================================

	InputSystemManager.cpp
	Created: 17 Jun 2018 5:57:00pm
	Author:  Ben

  ==============================================================================
*/

juce_ImplementSingleton(InputSystemManager)

InputSystemManager::InputSystemManager() :
	Thread("ISM"),
	isBeingDestroyed(false),
	inputQueuedNotifier(10)
{

	isInit = SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) == 0;

	if (!isInit)
	{
		LOGERROR("Could not init Input System Manager : " << SDL_GetError());
		return;
	}

	checkDevices();

	startThread();
	startTimerHz(1);
}

InputSystemManager::~InputSystemManager()
{
	isBeingDestroyed = true;
	stopTimer();
	stopThread(1000);
	SDL_Quit();
}

void InputSystemManager::checkDevices()
{
#if JUCE_MAC
    SDL_JoystickUpdate();
#endif
    
    int numDevices = SDL_NumJoysticks();
	for (int i = 0; i < numDevices; ++i)
	{
		if (SDL_IsGameController(i))
		{
			SDL_GameController* g = SDL_GameControllerOpen(i);
			if (g == NULL)
			{
				LOG("Unable to open gamepad : " << SDL_GetError());
				continue;
			}
			Gamepad* gp = getGamepadForSDL(g);
			if (gp == nullptr) addGamepad(new Gamepad(g));
		}
		else
		{
			SDL_Joystick* j = SDL_JoystickOpen(i);
			if (j == NULL)
			{
				LOG("Unable to open joystick : " << SDL_GetError());
				continue;
			}
			Gamepad* jj = getGamepadForSDL(j);
			if (jj == nullptr) addGamepad(new Gamepad(j));
		}
	}

	Array<Gamepad*> gamepadsToRemove;
	for (auto& g : gamepads)
	{
		//check removed devices
		if (g->joystick != nullptr)
		{
			if (!SDL_JoystickGetAttached(g->joystick)) gamepadsToRemove.add(g);
		}
		else
		{
			if (!SDL_GameControllerGetAttached(g->gamepad)) gamepadsToRemove.add(g);
		}
	}
	for (auto& g : gamepadsToRemove) removeGamepad(g);
}

Gamepad* InputSystemManager::addGamepad(Gamepad * g)
{
	gamepads.add(g);
	LOG("Gamepad added : " << g->getName());
	inputListeners.call(&InputManagerListener::gamepadAdded, g);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::GAMEPAD_ADDED, g));
	return g;
}

void InputSystemManager::removeGamepad(Gamepad* g)
{
	if (!gamepads.contains(g)) return;
	gamepads.removeObject(g, false);
	
	LOG("Gamepad removed : " << g->getName());
	
	inputListeners.call(&InputManagerListener::gamepadRemoved, g);
	inputQueuedNotifier.addMessage(new InputSystemEvent(InputSystemEvent::GAMEPAD_REMOVED, g));
	SDL_GameControllerClose(g->gamepad);
	delete g;

}


Gamepad* InputSystemManager::getGamepadForSDL(SDL_Joystick* tj)
{
	for (auto& g : gamepads) if (g->joystick != nullptr && g->joystick == tj) return g;
	return nullptr;
}
Gamepad* InputSystemManager::getGamepadForSDL(SDL_GameController* tg)
{
	for (auto& g : gamepads) if (g->gamepad == tg) return g;
	return nullptr;
}

Gamepad* InputSystemManager::getGamepadForID(SDL_JoystickGUID id)
{
	for (auto& g : gamepads)
	{
		SDL_JoystickGUID guid = g->joystick != nullptr?SDL_JoystickGetGUID(g->joystick): SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(g->gamepad));
		bool isTheSame = true;
		for (int i = 0; i < 16; ++i) if (guid.data[i] != id.data[i]) isTheSame = false;;
		if (isTheSame) return g;
	}
	return nullptr;
}

Gamepad* InputSystemManager::getGamepadForName(String name)
{
	for (auto& g : gamepads) if (String(SDL_GameControllerName(g->gamepad)) == name) return g;
	return nullptr;
}


void InputSystemManager::run()
{
	while (!threadShouldExit())
	{
		wait(20); //50fps

		if (Engine::mainEngine->isClearing || Engine::mainEngine->isLoadingFile) continue;
        
       // MessageManager::getInstance()->callFunctionOnMessageThread([](void *)->void*{SDL_Event e; SDL_PollEvent(&e); return nullptr;}, nullptr);
        
        int numDevices = SDL_NumJoysticks();
        if(numDevices != gamepads.size())
        {
            DBG("Gamepad num change !");
            continue;
        }
        SDL_JoystickUpdate();
		SDL_GameControllerUpdate();

		gamepads.getLock().enter();
		for (auto& g : gamepads)
		{
			if (g != nullptr) g->update();
		}
		gamepads.getLock().exit();
	}
}

void InputSystemManager::timerCallback()
{
    checkDevices();
}

Gamepad::Gamepad(SDL_GameController* gamepad) :
	gamepad(gamepad),
	joystick(nullptr)
{

}

Gamepad::Gamepad(SDL_Joystick* joystick) :
	gamepad(nullptr),
	joystick(joystick)
{
}

Gamepad::~Gamepad()
{
	masterReference.clear();
}

void Gamepad::update()
{
	int numAxes = joystick != nullptr ? SDL_JoystickNumAxes(joystick) : SDL_CONTROLLER_AXIS_MAX;
	int numButtons = joystick != nullptr ? SDL_JoystickNumButtons(joystick) : SDL_CONTROLLER_BUTTON_MAX;

	Array<float> axes;
	Array<bool> buttons;

	for (int i = 0; i < numAxes; ++i)
	{
		float val = joystick != nullptr ? (float)SDL_JoystickGetAxis(joystick, i) : (float)SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis)i);
		axes.add(val);
	}

	for (int i = 0; i < numButtons; ++i)
	{
		bool val = joystick != nullptr ? (SDL_JoystickGetButton(joystick, i) > 0) : (SDL_GameControllerGetButton(gamepad, (SDL_GameControllerButton)i) > 0);
		buttons.add(val);
	}

	gamepadListeners.call(&GamepadListener::gamepadValuesUpdated, axes, buttons);
}

SDL_JoystickID Gamepad::getDevID()
{
	return joystick != nullptr ? SDL_JoystickInstanceID(joystick) : SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gamepad));
}

String Gamepad::getName()
{
	return joystick != nullptr ? SDL_JoystickName(joystick) : SDL_GameControllerName(gamepad);
}

String Gamepad::getAxisName(int index)
{
	return "Axis " + String(index + 1);// : SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)index);
}

String Gamepad::getButtonName(int index)
{
	return "Button " + String(index + 1);//joystick != nullptr ? SDL_GameControllerGetStringForButton((SDL_GameControllerButton)index);
}

//GAMEPAD PARAMETER

GamepadParameter::GamepadParameter(const String& name, const String& description) :
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

void GamepadParameter::setGamepad(Gamepad* j)
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

	}
	else
	{
		var val;
		for (int i = 0; i < 16; ++i) val.append(0);
		setValue(val, false, true);
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

ControllableUI* GamepadParameter::createDefaultUI(Array<Controllable *> controllables)
{
	Array<GamepadParameter*> parameters = Inspectable::getArrayAs<Controllable, GamepadParameter>(controllables);
	if (parameters.size() == 0) parameters.add(this);
	return new GamepadParameterUI(parameters);
}
