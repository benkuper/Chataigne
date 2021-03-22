/*
  ==============================================================================

	JoyConModule.cpp
	Created: 23 Jan 2018 4:30:24pm
	Author:  Ben

  ==============================================================================
*/

JoyConModule::JoyConModule() :
	Module(getDefaultTypeString()),
	Thread("joycon"),
	leftValues("Left Controller"),
	rightValues("Right Controller")
{

	valuesCC.addChildControllableContainer(&leftValues);
	valuesCC.addChildControllableContainer(&rightValues);


	leftAccelX = leftValues.addFloatParameter("Left Accel X", "", 0, -1, 1);
	leftAccelY = leftValues.addFloatParameter("Left Accel Y", "", 0, -1, 1);
	leftAccelZ = leftValues.addFloatParameter("Left Accel Z", "", 0, -1, 1);

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

	rightAccelX = rightValues.addFloatParameter("Right Accel X", "", 0, -1, 1);
	rightAccelY = rightValues.addFloatParameter("Right Accel Y", "", 0, -1, 1);
	rightAccelZ = rightValues.addFloatParameter("Right Accel Z", "", 0, -1, 1);

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

	startThread();
}

JoyConModule::~JoyConModule()
{
	stopThread(1000);
}

#ifndef __arm__

void JoyConModule::updateController(int controller)
{

	//Left controller
	int type = JslGetControllerType(controller);
	
	JOY_SHOCK_STATE state = JslGetSimpleState(controller);
	MOTION_STATE motion = JslGetMotionState(controller);

	if (type == 1)
	{
		
		//Left Joycon
		up->setValue((state.buttons >> 0) & 1);
		down->setValue((state.buttons >> 1) & 1);
		left->setValue((state.buttons >> 2) & 1);
		right->setValue((state.buttons >> 3) & 1);

		minus->setValue((state.buttons >> 5) & 1);
		lStick->setValue((state.buttons >> 6) & 1);
		l->setValue((state.buttons >> 8) & 1);
		zl->setValue((state.buttons >> 10) & 1);

		capture->setValue((state.buttons >> 17) & 1);
		leftSL->setValue((state.buttons >> 18) & 1);
		leftSR->setValue((state.buttons >> 19) & 1);

		leftAccelX->setValue(motion.accelX);
		leftAccelY->setValue(motion.accelY);
		leftAccelZ->setValue(motion.accelZ);

		leftAxis->setPoint(state.stickLX, state.stickLY);
	} else if (type == 2)
	{
		//Right controller

		plus->setValue((state.buttons >> 4) & 1);
		rStick->setValue((state.buttons >> 7) & 1);
		r->setValue((state.buttons >> 9) & 1);
		zr->setValue((state.buttons >> 11) & 1);


		b->setValue((state.buttons >> 12) & 1);
		a->setValue((state.buttons >> 13) & 1);
		y->setValue((state.buttons >> 14) & 1);
		x->setValue((state.buttons >> 15) & 1);
		home->setValue((state.buttons >> 16) & 1);

		rightSL->setValue((state.buttons >> 18) & 1);
		rightSR->setValue((state.buttons >> 19) & 1);



		rightAccelX->setValue(motion.accelX);
		rightAccelX->setValue(motion.accelY);
		rightAccelX->setValue(motion.accelZ);

		rightAxis->setPoint(state.stickRX, state.stickRY);
	}
}

void JoyConModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c->parentContainer == &valuesCC || c->parentContainer->parentContainer == &valuesCC)
	{
		inActivityTrigger->trigger();
	}
}
#endif

void JoyConModule::run()
{
#ifndef __arm__

	int numConnected = JslConnectDevices();
	int connectedDevices[32];
	int deviceHandles = JslGetConnectedDeviceHandles(connectedDevices, numConnected);

	controllers.clear();

	for (int i = 0; i < deviceHandles;i++) {
		controllers.add(connectedDevices[i]);
		JslSetPlayerNumber(connectedDevices[i], i + 1);
	}
	
	LOG(controllers.size() << " controllers found.");

	if (controllers.size() > 0)
	{
		float rate = JslGetPollRate(0);
		int msToWait = 1000 / rate;

		while (!threadShouldExit())
		{
			try
			{
				for (auto& controller : controllers) updateController(controller);
				wait(msToWait);
			}
			catch (std::exception e)
			{
				LOGERROR("Error while updating the joycon : " << e.what());
				break;
			}
		}
	}
	
	controllers.clear();
	JslDisconnectAndDisposeAll();
#endif

}
