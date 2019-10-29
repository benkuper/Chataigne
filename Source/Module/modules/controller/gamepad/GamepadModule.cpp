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
	setupIOConfiguration(true, false);
	gamepadParam = new GamepadParameter("Device", "The Gamepad to connect to");
	moduleParams.addParameter(gamepadParam);
	InputSystemManager::getInstance()->addInputManagerListener(this);
}

GamepadModule::~GamepadModule()
{
	if(InputSystemManager::getInstanceWithoutCreating() != nullptr) InputSystemManager::getInstance()->removeInputManagerListener(this);
}

void GamepadModule::rebuildValues()
{
	valuesCC.clear();
	if (gamepadParam->gamepad == nullptr) return;
	valuesCC.addChildControllableContainer(&gamepadParam->gamepad->axesCC);
	valuesCC.addChildControllableContainer(&gamepadParam->gamepad->buttonsCC);
}


void GamepadModule::gamepadAdded(Gamepad * g)
{
	String gName = String(SDL_GameControllerName(g->gamepad));
	if (gName == gamepadParam->ghostName)
	{
		gamepadParam->setGamepad(g);
		rebuildValues();
	}
}


void GamepadModule::gamepadRemoved(Gamepad * g)
{
	if (gamepadParam->gamepad == g)
	{
		valuesCC.removeChildControllableContainer(&gamepadParam->gamepad->axesCC);
		valuesCC.removeChildControllableContainer(&gamepadParam->gamepad->buttonsCC);
	}
}

void GamepadModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == gamepadParam)
	{
		rebuildValues();
	}
}
