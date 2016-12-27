/*
  ==============================================================================

    GamepadManager.cpp
    Created: 26 Dec 2016 5:56:42pm
    Author:  Ben

  ==============================================================================
*/

#include "GamepadManager.h"

juce_ImplementSingleton(GamepadManager)

GamepadManager::GamepadManager() :
	Thread("gamepad")
{
	DBG("Gamepad Manager :: init");

	/*
		
	manager.Update();
	startThread();

	
	
	for (auto &dMap : manager)
	{
		DBG(dMap.first << " : " << String(dMap.second->GetTypeName()));
	}
	*/
}

GamepadManager::~GamepadManager()
{
	stopThread(100);
}

void GamepadManager::run()
{
	/*
	while (!threadShouldExit())
	{
		manager.Update();
		for (auto &dMap : manager)
		{
			DBG(dMap.first << " : " << String(dMap.second->GetTypeName()));
		}
		sleep(10);
	}
	*/
}
