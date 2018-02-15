/*
  ==============================================================================

    GamepadModule.h
    Created: 26 Dec 2016 4:56:31pm
    Author:  Ben

  ==============================================================================
*/

#ifndef GAMEPADMODULE_H_INCLUDED
#define GAMEPADMODULE_H_INCLUDED

#include "Module/Module.h"
//#include "gainput\gainput.h"
#include "GamepadManager.h"

//using namespace gainput;

//#include "gamepad\Gamepad.h"

/*
void deviceAddedStaticCallback(Gamepad_device * device, void * context);
void deviceRemovedStaticCallback(Gamepad_device * device, void * context);
*/



class GamepadModule :
	public Module,
	public MultiTimer
{
public:
	GamepadModule(const String &name = "Gamepad");
	~GamepadModule();


	void checkDevices();
	/*
	void deviceAddedCallback(Gamepad_device * device);
	void deviceRemovedCallback(Gamepad_device * device);
	*/
	
	
	void timerCallback(int timerID) override;


	class GamepadListener
	{
	public:
		virtual ~GamepadListener() {}
		virtual void deviceAdded() {}
		virtual void deviceRemoved() {}
	};

	ListenerList<GamepadListener> gamepadListeners;
	void addGamepadListener(GamepadListener* newListener) { gamepadListeners.add(newListener); }
	void removeGamepadListener(GamepadListener* listener) { gamepadListeners.remove(listener); }


	static GamepadModule * create() { return new GamepadModule(); }
	virtual String getDefaultTypeString() const override { return "Gamepad"; }

};




#endif  // GAMEPADMODULE_H_INCLUDED
