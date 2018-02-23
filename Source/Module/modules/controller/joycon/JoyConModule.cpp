/*
  ==============================================================================

	JoyConModule.cpp
	Created: 23 Jan 2018 4:30:24pm
	Author:  Ben

  ==============================================================================
*/

#include "JoyConModule.h"

JoyConModule::JoyConModule() :
	Module(getDefaultTypeString()),
	Thread("joycon"),
	leftValues("Left Controller"),
	rightValues("Right Controller"),
	imuParams("IMU"),
    lastIMUProcessTime(0),
    resetIMUOnNextPacket(true),
    imuError(0)
{
	startThread();

	moduleParams.addChildControllableContainer(&imuParams);
	imuFilterWeight = imuParams.addFloatParameter("IMU Filter Weight", "Filter for the IMU computation", 0.01f, 0, 1);
	resetIMU = imuParams.addTrigger("Reset IMU", "Recalibrate IMU. Controller should be standing still on a flat surface when doing that");

	valuesCC.addChildControllableContainer(&leftValues);
	valuesCC.addChildControllableContainer(&rightValues);


	leftRoll = leftValues.addFloatParameter("Left Roll", "", 0, -1, 1);
	leftPitch = leftValues.addFloatParameter("Left Pitch", "", 0, -1, 1);
	leftAxis = leftValues.addPoint2DParameter("Left Axis", "");
	leftAxis->setBounds(-1, -1, 1, 1);
	lStick = leftValues.addBoolParameter("Left Stick", "", false);
	up = leftValues.addBoolParameter("Up", "", false);
	down = leftValues.addBoolParameter("Down", "", false);
	left = leftValues.addBoolParameter("Left", "", false);
	right = leftValues.addBoolParameter("Right", "", false);
	l = leftValues.addBoolParameter("L", "", false);
	zl = leftValues.addBoolParameter("ZL", "", false);
	leftSL = leftValues.addBoolParameter("Left SL", "", false);
	leftSR = leftValues.addBoolParameter("Left SR", "", false);
	minus = leftValues.addBoolParameter("-", "", false);
	capture = leftValues.addBoolParameter("Capture", "", false);

	rightRoll = rightValues.addFloatParameter("Right Roll", "", 0, -1, 1);
	rightPitch = rightValues.addFloatParameter("Right Pitch", "", 0, -1, 1);
	rightAxis = rightValues.addPoint2DParameter("Right Axis", "");
	rightAxis->setBounds(-1, -1, 1, 1);
	rStick = rightValues.addBoolParameter("Right Stick", "", false);
	a = rightValues.addBoolParameter("A", "", false);
	b = rightValues.addBoolParameter("B", "", false);
	x = rightValues.addBoolParameter("X", "", false);
	y = rightValues.addBoolParameter("Y", "", false);
	r = rightValues.addBoolParameter("R", "", false);
	zr = rightValues.addBoolParameter("ZR", "", false);
	rightSL = rightValues.addBoolParameter("Right SL", "", false);
	rightSR = rightValues.addBoolParameter("Right SR", "", false);
	plus = rightValues.addBoolParameter("+", "", false);
	home = rightValues.addBoolParameter("Home", "", false);

	for (auto &c : leftValues.controllables) c->isControllableFeedbackOnly = true;
	for (auto &c : rightValues.controllables) c->isControllableFeedbackOnly = true;
}

JoyConModule::~JoyConModule()
{
	signalThreadShouldExit();
	stopThread(1000);
}

#if JUCE_WINDOWS

void JoyConModule::updateController(Joytime::Controller * controller)
{
	controller->update();

	//Left controller
	if (controller->type == Joytime::ControllerType::LeftJoycon)
	{
		capture->setValue(controller->buttons.capture);
		down->setValue(controller->buttons.down);
		left->setValue(controller->buttons.left);
		right->setValue(controller->buttons.right);
		up->setValue(controller->buttons.up);
		l->setValue(controller->buttons.l);
		leftSL->setValue(controller->buttons.sl);
		leftSR->setValue(controller->buttons.sr);
		lStick->setValue(controller->buttons.lStick);
		zl->setValue(controller->buttons.zl);
		minus->setValue(controller->buttons.minus);
		float tx = 0;
		float ty = 0;
		if (abs(controller->leftStick.x) > controller->leftStickCalibration.deadZone)
		{
			if (controller->leftStick.x > 0) tx = jmap<float>((float)controller->leftStick.x, (float)controller->leftStickCalibration.deadZone, (float)controller->leftStickCalibration.xMax - 100, 0, 1);
			else tx = jmap<float>((float)controller->leftStick.x, (float)-controller->leftStickCalibration.xMax + 150, (float)-controller->leftStickCalibration.deadZone, -1, 0);
		}

		if (abs(controller->leftStick.y) > controller->leftStickCalibration.deadZone)
		{
			if (controller->leftStick.y > 0) ty = jmap<float>((float)controller->leftStick.y, (float)controller->leftStickCalibration.deadZone, (float)controller->leftStickCalibration.yMax - 5, 0, 1);
			else ty = jmap<float>((float)controller->leftStick.y, (float)-controller->leftStickCalibration.yMax + 100, (float)-controller->leftStickCalibration.deadZone, -1, 0);
		}

		leftAxis->setPoint(tx, ty);
	} else if (controller->type == Joytime::ControllerType::RightJoycon)
	{
		//Right controller
		a->setValue(controller->buttons.a);
		b->setValue(controller->buttons.b);
		x->setValue(controller->buttons.x);
		y->setValue(controller->buttons.y);
		r->setValue(controller->buttons.r);
		rightSL->setValue(controller->buttons.sl);
		rightSR->setValue(controller->buttons.sr);
		rStick->setValue(controller->buttons.rStick);
		zr->setValue(controller->buttons.zr);
		plus->setValue(controller->buttons.plus);
		home->setValue(controller->buttons.home);

		float tx = 0;
		float ty = 0;
		if (abs(controller->rightStick.x) > controller->rightStickCalibration.deadZone)
		{
			if (controller->rightStick.x > 0) tx = jmap<float>((float)controller->rightStick.x, (float)controller->rightStickCalibration.deadZone, (float)controller->rightStickCalibration.xMax - 5, 0, 1);
			else tx = jmap<float>((float)controller->rightStick.x, (float)-controller->rightStickCalibration.xMax + 100, (float)-controller->rightStickCalibration.deadZone, -1, 0);
		}

		if (abs(controller->rightStick.y) > controller->leftStickCalibration.deadZone)
		{
			if (controller->rightStick.y > 0) ty = jmap<float>((float)controller->rightStick.y, (float)controller->rightStickCalibration.deadZone, (float)controller->rightStickCalibration.yMax - 5, 0, 1);
			else ty = jmap<float>((float)controller->rightStick.y, (float)-controller->rightStickCalibration.yMax + 100, (float)-controller->rightStickCalibration.deadZone, -1, 0);
		}

		rightAxis->setPoint(tx, ty);
	}

	processIMU(controller);
}


void JoyConModule::processIMU(Joytime::Controller * controller)
{
	if (!imuParams.enabled->boolValue()) return;
}

void JoyConModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == imuParams.enabled)
	{
		for (auto &controller : controllers) controller->setSixAxisEnabled(imuParams.enabled->boolValue());
	} else if (c->parentContainer == &valuesCC || c->parentContainer->parentContainer == &valuesCC)
	{
		inActivityTrigger->trigger();
	}
}
void JoyConModule::onContainerTriggerTriggered(Trigger * t)
{
	Module::onContainerTriggerTriggered(t);
	if (t == resetIMU) resetIMUOnNextPacket = true;
}

#endif

void JoyConModule::run()
{
#if JUCE_WINDOWS
	std::vector<Joytime::Controller> cVector = Joytime::scanForControllers();

	controllers.clear();

	for (Joytime::Controller& controller : cVector) {
		controllers.add(&controller);

		Joytime::initController(&controller, true);
		controller.setLEDs(Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::On, Joytime::ControllerLEDState::Off, Joytime::ControllerLEDState::On);
	}

	while (!threadShouldExit())
	{
		for (auto &controller : controllers) updateController(controller);
		sleep(15);
	}

	controllers.clear();
#endif

}
