/*
  ==============================================================================

	InputSystemManager.h
	Created: 17 Jun 2018 5:57:00pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SDL.h"

class Gamepad
{
public:
	Gamepad(SDL_GameController* gamepad);
	Gamepad(SDL_Joystick* joystick);
	virtual ~Gamepad();

	SDL_GameController* gamepad;
	SDL_Joystick* joystick;

	//ControllableContainer axesCC;
	//ControllableContainer buttonsCC;

	//float axisOffset[SDL_JOYSTICK_AXIS_MAX];
	//float axisDeadZone[SDL_JOYSTICK_AXIS_MAX];

	virtual void update();
	SDL_JoystickID getDevID();

	String getName();

	static String getAxisName(int index);
	static String getButtonName(int index);

	class GamepadListener
	{
	public:
		virtual ~GamepadListener() {}
		virtual void gamepadValuesUpdated(Array<float> axes, Array<bool> buttons) {}
	};

	ListenerList<GamepadListener> gamepadListeners;
	void addGamepadListener(GamepadListener* newListener) { gamepadListeners.add(newListener); }
	void removeGamepadListener(GamepadListener* listener) { gamepadListeners.remove(listener); }


	WeakReference<Gamepad>::Master masterReference;
};

class GamepadParameter :
	public Parameter
{
public:
	GamepadParameter(const String& name, const String& description);
	~GamepadParameter();

	WeakReference<Gamepad> gamepad;

	//Ghosting
	SDL_JoystickGUID ghostID;
	String ghostName;

	void setGamepad(Gamepad* j);

	var getJSONDataInternal() override;
	void loadJSONDataInternal(var data) override;

	// Inherited via Parameter
	virtual ControllableUI* createDefaultUI(Array<Controllable*> controllables = {}) override;

	String getTypeString() const override { return "GamepadParam"; }
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

	//OwnedArray<Joystick, CriticalSection> joysticks;
	OwnedArray<Gamepad, CriticalSection> gamepads;

	Gamepad* addGamepad(Gamepad * controller);
	void removeGamepad(Gamepad* g);
	//Gamepad* addJoystick(SDL_Joystick * sdlJ);
	//void removeJoystick(Gamepad * j);

	Gamepad* getGamepadForSDL(SDL_GameController* g);
	Gamepad* getGamepadForSDL(SDL_Joystick* j);
	Gamepad* getGamepadForID(SDL_JoystickGUID id);
	Gamepad* getGamepadForName(String name);

	void run() override;


	class InputManagerListener
	{
	public:
		virtual ~InputManagerListener() {}
		virtual void gamepadAdded(Gamepad* info) {}
		virtual void gamepadRemoved(Gamepad* info) {}
	};

	ListenerList<InputManagerListener> inputListeners;
	void addInputManagerListener(InputManagerListener* newListener) { inputListeners.add(newListener); }
	void removeInputManagerListener(InputManagerListener* listener) { inputListeners.remove(listener); }


	class  InputSystemEvent
	{
	public:
		enum Type { GAMEPAD_ADDED, GAMEPAD_REMOVED };

		InputSystemEvent(Type t, Gamepad* g) : type(t), gamepad(g), devID(g->getDevID()) {}

		Type type;
		Gamepad* gamepad;
		SDL_JoystickID devID;
	};

	QueuedNotifier<InputSystemEvent> inputQueuedNotifier;
	typedef QueuedNotifier<InputSystemEvent>::Listener AsyncListener;


	void addAsyncInputListener(AsyncListener* newListener) { inputQueuedNotifier.addListener(newListener); }
	void addAsyncCoalescedInputListener(AsyncListener* newListener) { inputQueuedNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncInputListener(AsyncListener* listener) { inputQueuedNotifier.removeListener(listener); }
};
