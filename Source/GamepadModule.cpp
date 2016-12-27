/*
  ==============================================================================

    GamepadModule.cpp
    Created: 26 Dec 2016 4:56:31pm
    Author:  Ben

  ==============================================================================
*/

#include "GamepadModule.h"

GamepadModule::GamepadModule(const String & name) :
	Module(name)
{
	
	GamepadManager::getInstance();


	/*
	Gamepad_deviceAttachFunc(deviceAddedStaticCallback, this);
	Gamepad_deviceAttachFunc(deviceRemovedStaticCallback, this);

	Gamepad_init();
	Gamepad_detectDevices();
	*/
}

GamepadModule::~GamepadModule()
{
}

void GamepadModule::checkDevices()
{
}

/*
void GamepadModule::deviceAddedCallback(Gamepad_device * device)
{
	DBG("Device added : " << String(device->description));
}

void GamepadModule::deviceRemovedCallback(Gamepad_device * device)
{
}
*/
void GamepadModule::timerCallback(int /*timerID*/)
{
}

/*
void deviceAddedStaticCallback(Gamepad_device * device, void * context)
{
	GamepadModule * m = reinterpret_cast<GamepadModule *>(context);
}


void deviceRemovedStaticCallback(Gamepad_device * device, void * context)
{
}
*/
