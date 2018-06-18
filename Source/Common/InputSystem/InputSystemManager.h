/*
  ==============================================================================

    InputSystemManager.h
    Created: 17 Jun 2018 5:57:00pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#if JUCE_WINDOWS
#include "SDL.h"


class Joystick
{
public:
	Joystick(SDL_Joystick * joystick);
	~Joystick();

	SDL_Joystick * joystick;

	ControllableContainer axesCC;
	ControllableContainer buttonsCC;

	virtual void update();


	WeakReference<Joystick>::Master masterReference;
};

class Gamepad
{
public:
	Gamepad(SDL_GameController * gamepad);
	~Gamepad();

	SDL_GameController * gamepad;

	ControllableContainer axesCC;
	ControllableContainer buttonsCC;

	virtual void update();


	WeakReference<Gamepad>::Master masterReference;
};

class JoystickParameter :
	public Parameter
{
public:
	JoystickParameter(const String &name, const String &description);
	~JoystickParameter();

	WeakReference<Joystick> joystick;

	//Ghosting
	SDL_JoystickGUID ghostID;
	String ghostName;

	void setJoystick(Joystick * j);

	var getJSONDataInternal() override;
	void loadJSONDataInternal(var data) override;


	// Inherited via Parameter
	virtual ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

};



class GamepadParameter :
	public Parameter
{
public:
	GamepadParameter(const String &name, const String &description);
	~GamepadParameter();

	WeakReference<Gamepad> gamepad;

	//Ghosting
	SDL_JoystickGUID ghostID;
	String ghostName;

	void setGamepad(Gamepad * j);

	var getJSONDataInternal() override;
	void loadJSONDataInternal(var data) override;

	// Inherited via Parameter
	virtual ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

};


class InputSystemManager :
	public Thread
{
public:
	juce_DeclareSingleton(InputSystemManager, true);

	InputSystemManager();
	~InputSystemManager();

	bool isInit;
	const uint32 checkDeviceTime = 1000; //ms
	uint32 lastCheckTime;

	OwnedArray<Joystick> joysticks;
	OwnedArray<Gamepad> gamepads;

	Gamepad * addGamepad(SDL_GameController * controller);
	void removeGamepad(Gamepad * g);
	Joystick * addJoystick(SDL_Joystick * sdlJ);
	void removeJoystick(Joystick * j);

	Joystick * getJoystickForSDL(SDL_Joystick * j);
	Gamepad * getGamepadForSDL(SDL_GameController * g);
	Joystick * getJoystickForID(SDL_JoystickGUID id);
	Gamepad * getGamepadForID(SDL_JoystickGUID id);
	Joystick * getJoystickForName(String name);
	Gamepad * getGamepadForName(String name);

	void run() override;


	class InputManagerListener
	{
	public:
		virtual ~InputManagerListener() {}
		virtual void joystickAdded(Joystick  *) {}
		virtual void joystickRemoved(Joystick  *) {}
		virtual void gamepadAdded(Gamepad  *info) {}
		virtual void gamepadRemoved(Gamepad  *info) {}
	};

	ListenerList<InputManagerListener> inputListeners;
	void addInputManagerListener(InputManagerListener* newListener) { inputListeners.add(newListener); }
	void removeInputManagerListener(InputManagerListener* listener) { inputListeners.remove(listener); }


	class  InputSystemEvent
	{
	public:
		enum Type { JOYSTICK_ADDED, JOYSTICK_REMOVED, GAMEPAD_ADDED, GAMEPAD_REMOVED };

		InputSystemEvent(Type t, Joystick * j) : type(t), joystick(j), gamepad(nullptr), devID(SDL_JoystickInstanceID(j->joystick)) {}
		InputSystemEvent(Type t, Gamepad * g) : type(t),  joystick(nullptr), gamepad(g), devID(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(g->gamepad))) {}

		Type type;
		Joystick * joystick;
		Gamepad * gamepad;
		SDL_JoystickID devID;
	};

	QueuedNotifier<InputSystemEvent> inputQueuedNotifier;
	typedef QueuedNotifier<InputSystemEvent>::Listener AsyncListener;


	void addAsyncInputListener(AsyncListener* newListener) { inputQueuedNotifier.addListener(newListener); }
	void addAsyncCoalescedInputListener(AsyncListener* newListener) { inputQueuedNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncInputListener(AsyncListener* listener) { inputQueuedNotifier.removeListener(listener); }
};


#endif