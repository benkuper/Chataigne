/*
  ==============================================================================

    GamepadManager.h
    Created: 26 Dec 2016 5:56:42pm
    Author:  Ben

  ==============================================================================
*/

#ifndef GAMEPADMANAGER_H_INCLUDED
#define GAMEPADMANAGER_H_INCLUDED

#include "JuceHeader.h"
//#include "gainput\gainput.h"

class GamepadManager :
	public Thread
{
public:
	juce_DeclareSingleton(GamepadManager,true);
	GamepadManager();
	virtual ~GamepadManager();


	//gainput::InputManager manager;


	// Inherited via Thread
	virtual void run() override;

};




#endif  // GAMEPADMANAGER_H_INCLUDED
