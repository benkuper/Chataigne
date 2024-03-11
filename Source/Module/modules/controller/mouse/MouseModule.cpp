/*
  ==============================================================================

    MouseModule.cpp
    Created: 12 Mar 2020 3:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#if JUCE_WINDOWS
#include <windows.h>
#define LEFT_DOWN MOUSEEVENTF_LEFTDOWN
#define LEFT_UP MOUSEEVENTF_LEFTUP
#define MIDDLE_DOWN MOUSEEVENTF_MIDDLEDOWN
#define MIDDLE_UP MOUSEEVENTF_MIDDLEUP
#define RIGHT_DOWN MOUSEEVENTF_RIGHTDOWN
#define RIGHT_UP MOUSEEVENTF_RIGHTUP
#define WHEEL MOUSEEVENTF_WHEEL
#define HWHEEL MOUSEEVENTF_HWHEEL
#elif JUCE_MAC
#include "MouseMacFunctions.h"
#if JUCE_SUPPORT_CARBON
#define LEFT_DOWN kCGEventLeftMouseDown
#define LEFT_UP kCGEventLeftMouseUp
#define MIDDLE_DOWN kCGEventOtherMouseDown
#define MIDDLE_UP kCGEventOtherMouseUp
#define RIGHT_DOWN kCGEventRightMouseDown
#define RIGHT_UP kCGEventRightMouseUp
#endif
#endif

#ifndef LEFT_DOWN
#define LEFT_DOWN 0
#define LEFT_UP 1
#define MIDDLE_DOWN 2
#define MIDDLE_UP 3
#define RIGHT_DOWN 4
#define RIGHT_UP 5
#define WHEEL 6
#define HWHEEL 7
#endif


MouseModule::MouseModule() : 
	Module(getTypeString())
{
	setupIOConfiguration(true, true);

	updateRate = moduleParams.addIntParameter("Update rate", "The rate at which to update the mouse informations", 50, 1, 200);

	currentScreen = valuesCC.addIntParameter("Current screen", "The index of the screen the pointer is currently on", 0, 0);
	absolutePosition = valuesCC.addPoint2DParameter("Absolute position", "The absolute mouse position from across all screens");
	relativePosition = valuesCC.addPoint2DParameter("Relative position", "The relative mouse position in the current screen");
	leftButtonDown = valuesCC.addBoolParameter("Left button", "Is left button down ?", false);
	middleButtonDown = valuesCC.addBoolParameter("Middle button", "Is middle button down ?", false);
	rightButtonDown = valuesCC.addBoolParameter("Right button", "Is right button down ?", false);
	wheelYDelta = valuesCC.addFloatParameter("Wheel Delta - Vertical", "Data received from a vertical mouse wheel, negative for down.", 0, -2, 2);
	wheelYData = valuesCC.addTrigger("Wheel Data - Vertical", "Has data been received from the vertical mouse wheel?");
	wheelXDelta = valuesCC.addFloatParameter("Wheel Delta - Horizontal", "Data received from a horizontal mouse wheel, negative for right.", 0, -2, 2);
	wheelXData = valuesCC.addTrigger("Wheel Data - Horizontal", "Has data been received from the horizontal mouse wheel?");

	Desktop::getInstance().addGlobalMouseListener(this);

	defManager->add(CommandDefinition::createDef(this, "", "Set Cursor Pos", &MouseModuleCommands::create)->addParam("type", MouseModuleCommands::SET_CURSOR_POSITION));
	defManager->add(CommandDefinition::createDef(this, "", "Button Down", &MouseModuleCommands::create)->addParam("type", MouseModuleCommands::BUTTON_DOWN));
	defManager->add(CommandDefinition::createDef(this, "", "Button Up", &MouseModuleCommands::create)->addParam("type", MouseModuleCommands::BUTTON_UP));
	defManager->add(CommandDefinition::createDef(this, "", "Button Click",&MouseModuleCommands::create)->addParam("type", MouseModuleCommands::BUTTON_CLICK));
	defManager->add(CommandDefinition::createDef(this, "", "Mouse Wheel", &MouseModuleCommands::create)->addParam("type", MouseModuleCommands::MOUSE_WHEEL));

	startTimerHz(updateRate->intValue());
}

MouseModule::~MouseModule()
{
}

void MouseModule::clearItem()
{
	Desktop::getInstance().removeGlobalMouseListener(this);
}

void MouseModule::setCursorPosition(Point<float>& pos, bool isRelative)
{
	if (!enabled->boolValue()) return;
	
	outActivityTrigger->trigger();

	MessageManagerLock mmLock;
	if (isRelative)
	{
		if (const Displays::Display* d = Desktop::getInstance().getDisplays().getPrimaryDisplay())
		{
			juce::Rectangle<int> r = d->totalArea;
			pos *= Point<float>((float)r.getWidth(), (float)r.getHeight());
		}
	}

	Desktop::getInstance().getMainMouseSource().setScreenPosition(pos);
}

void MouseModule::setWheelData(float wheelDelta, int orientation)
{
	if (!enabled->boolValue()) return;
	outActivityTrigger->trigger();
	NLOG(niceName, "Sent delta of " << wheelDelta << " to " << (orientation == 1 ? "Vertical Wheel" : "Horizontal Wheel"));
	int wheelType = (orientation == 1 ? WHEEL : HWHEEL);
	int winWheelTravel = wheelDelta * 515;
#if JUCE_WINDOWS
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = wheelType;
	Input.mi.mouseData = winWheelTravel;
	::SendInput(1, &Input, sizeof(INPUT));
//#elif JUCE_MAC
//	mousemac::sendScrollWheelEvent(buttonEvent, pos.x, pos.y);
#endif
}

void MouseModule::setButtonDown(int buttonID)
{
	if (!enabled->boolValue()) return;
	outActivityTrigger->trigger();
	sendButtonEvent(buttonID == 0 ? LEFT_DOWN : (buttonID == 1 ? MIDDLE_DOWN : RIGHT_DOWN));
}

void MouseModule::setButtonUp(int buttonID)
{
	if (!enabled->boolValue()) return;
	outActivityTrigger->trigger();
	sendButtonEvent(buttonID == 0 ? LEFT_UP : (buttonID == 1 ? MIDDLE_UP : RIGHT_UP));
}

void MouseModule::setButtonClick(int buttonID)
{
	if (!enabled->boolValue()) return;
	setButtonDown(buttonID);
	setButtonUp(buttonID);
}

void MouseModule::sendButtonEvent(int buttonEvent)
{
	if (!enabled->boolValue()) return;

#if JUCE_WINDOWS
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = buttonEvent;
	::SendInput(1, &Input, sizeof(INPUT));
#elif JUCE_MAC
    juce::Point<float> pos = Desktop::getInstance().getMainMouseSource().getScreenPosition();
    mousemac::sendMouseEvent(buttonEvent, pos.x, pos.y);
#endif
}

void MouseModule::mouseDown(const MouseEvent& e)
{
	if (!enabled->boolValue()) return;
	
	inActivityTrigger->trigger();
	leftButtonDown->setValue(e.mods.isLeftButtonDown());
	middleButtonDown->setValue(e.mods.isMiddleButtonDown());
	rightButtonDown->setValue(e.mods.isRightButtonDown());
}


void MouseModule::mouseUp(const MouseEvent& e)
{
	if (!enabled->boolValue()) return;
	
	inActivityTrigger->trigger();
	leftButtonDown->setValue(false);
	middleButtonDown->setValue(false);
	rightButtonDown->setValue(false);
}

void MouseModule::mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& d)
{
	if (!enabled->boolValue()) return;

	inActivityTrigger->trigger();
	int reversed = d.isReversed ? -1 : 1;
	String revStr = d.isReversed ? "True" : "False";
	if (d.deltaY != 0) {
		wheelYDelta->setValue(d.deltaY * reversed);
		wheelYData->trigger();
		if (logIncomingData->boolValue()) NLOG(niceName, "Y Delta " << d.deltaY << " received. Reversed: " << revStr);
	}
	if (d.deltaX != 0) {
		wheelXDelta->setValue(d.deltaX * reversed);
		wheelXData->trigger();
		if (logIncomingData->boolValue()) NLOG(niceName, "X Delta " << d.deltaX << " received. Reversed: " << revStr);
	}
}

void MouseModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) startTimerHz(updateRate->intValue());
		else stopTimer();
	}
}

void MouseModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == updateRate && enabled->boolValue()) startTimerHz(updateRate->intValue());
}

void MouseModule::updateMouseInfos()
{
	if (!enabled->boolValue()) return;

	Point<float> pos = Desktop::getInstance().getMainMouseSource().getScreenPosition();
	absolutePosition->setPoint(pos);

	RectangleList<int> areas = Desktop::getInstance().getDisplays().getRectangleList(false);
	for (int i = 0; i < areas.getNumRectangles(); ++i)
	{
		juce::Rectangle<int> r = areas.getRectangle(i);
		if (r.contains(pos.toInt()))
		{
			currentScreen->setValue(i);
			Point<float> sp = pos- r.getTopLeft().toFloat();
			relativePosition->setPoint(sp.x / r.getWidth(), sp.y / r.getHeight());

		}
	}
}

void MouseModule::timerCallback()
{
	updateMouseInfos();
}
