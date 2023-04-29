/*
  ==============================================================================

	GamepadModule.cpp
	Created: 26 Dec 2016 4:56:31pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

GamepadModule::GamepadModule(const String& name) :
	Module(name),
	gamepad(nullptr),
	axesCC("Axes"),
	buttonsCC("Buttons"),
	calibCC("Calibration")
{
	setupIOConfiguration(true, false);
	includeValuesInSave = true;

	gamepadParam = new GamepadParameter("Device", "The Gamepad to connect to");
	moduleParams.addParameter(gamepadParam);

	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
	{
		FloatParameter* f = axesCC.addFloatParameter(Gamepad::getAxisName(i), "", 0, -1, 1);
		f->isControllableFeedbackOnly = true;

		axisOffset.add(calibCC.addFloatParameter("Axis " + String(i + 1) + " Offset", "Offset if axis is not centered", 0, -1, 1));
		axisDeadzone.add(calibCC.addFloatParameter("Axis " + String(i + 1) + " Dead zone", "Percentage of dead zone in the center to avoid noisy input", 0, 0, 1));
	}

	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		BoolParameter* bp = buttonsCC.addBoolParameter(Gamepad::getButtonName(i), "", false);
		bp->isControllableFeedbackOnly = true;
	}

	moduleParams.addChildControllableContainer(&calibCC);

	valuesCC.addChildControllableContainer(&axesCC);
	valuesCC.addChildControllableContainer(&buttonsCC);

	InputSystemManager::getInstance()->addInputManagerListener(this);
}

GamepadModule::~GamepadModule()
{
	gamepadParam->setGamepad(nullptr);
	if (InputSystemManager::getInstanceWithoutCreating() != nullptr) InputSystemManager::getInstance()->removeInputManagerListener(this);
}

void GamepadModule::setGamepad(Gamepad* g)
{
	if (gamepad == g) return;
	if (!gamepadRef.wasObjectDeleted() && gamepad != nullptr) gamepad->removeGamepadListener(this);

	gamepad = g;
	gamepadRef = g;

	if (gamepad != nullptr) gamepad->addGamepadListener(this);
}

void GamepadModule::gamepadAdded(Gamepad* g)
{
	String gName = String(SDL_GameControllerName(g->gamepad));
	if (gName == gamepadParam->ghostName) gamepadParam->setGamepad(g);
}


void GamepadModule::gamepadRemoved(Gamepad* g)
{
	if (g == gamepad) gamepadParam->setGamepad(nullptr);
}

void GamepadModule::gamepadValuesUpdated(Array<float> axes, Array<bool> buttons)
{
	for (int i = 0; i < axes.size() && i < axesCC.controllables.size(); ++i)
	{
		float axisValue = jmap<float>(axes[i], INT16_MIN, INT16_MAX, -1, 1) + axisOffset[i]->floatValue();
		if (fabs(axisValue) < axisDeadzone[i]->floatValue()) axisValue = 0;
		else
		{
			if (axisValue > 0) axisValue = jmap<float>(axisValue, axisDeadzone[i]->floatValue(), 1 + axisOffset[i]->floatValue(), 0, 1);
			else axisValue = jmap<float>(axisValue, -1 + axisOffset[i]->floatValue(), -axisDeadzone[i]->floatValue(), -1, 0);
		}
		((FloatParameter*)axesCC.controllables[i])->setValue(axisValue);
	}

	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
	{
		((BoolParameter*)buttonsCC.controllables[i])->setValue(buttons[i]);
	}
}

void GamepadModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == gamepadParam) setGamepad(gamepadParam->gamepad);
}
