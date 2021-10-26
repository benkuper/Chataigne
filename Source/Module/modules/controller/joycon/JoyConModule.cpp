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

	reconnectControllers = moduleParams.addTrigger("Reconnect Controllers", "If the controllers were not powered on at initialization, then you can connect by clicking here");

	leftAccel = leftValues.addPoint3DParameter("Left Accel", "");
	leftAccel->setBounds(-1, -1, -1, 1, 1, 1);
	leftOrientation = leftValues.addPoint3DParameter("Left Orientation", "");
	leftOrientation->setBounds(-1, -1, -1, 1, 1, 1);
	
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

	rightAccel = rightValues.addPoint3DParameter("Right Accel", "");
	rightAccel->setBounds(-1, -1, -1, 1, 1, 1);
	rightOrientation = rightValues.addPoint3DParameter("Right Orientation", "");
	rightOrientation->setBounds(-1, -1, -1, 1, 1, 1);
	
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

void JoyConModule::connectControllers()
{
	
	LOG("Connecting Joycons...");
	int numConnected = JslConnectDevices();
	int connectedDevices[32];
	int deviceHandles = JslGetConnectedDeviceHandles(connectedDevices, numConnected);

	controllers.clear();

	for (int i = 0; i < deviceHandles; i++) {
		controllers.add(connectedDevices[i]);
		JslSetPlayerNumber(connectedDevices[i], i + 1);
	}

	
	LOG(controllers.size() << " controllers found.");
}

void JoyConModule::updateController(int controller)
{
	if (isClearing || !enabled->boolValue()) return;

	//Left controller
	int type = JslGetControllerType(controller);
	
	JOY_SHOCK_STATE state = JslGetSimpleState(controller);
	MOTION_STATE motion = JslGetMotionState(controller);

	if (type == 1)
	{
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

		leftAccel->setVector(motion.accelX, motion.accelY, motion.accelZ);
		leftOrientation->setVector(motion.gravX, motion.gravY, motion.gravZ);

		leftAxis->setPoint(state.stickLX, state.stickLY);
	} else if (type == 2)
	{
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

		rightAccel->setVector(motion.accelX, motion.accelY, motion.accelZ);
		rightOrientation->setVector(motion.gravX, motion.gravY, motion.gravZ);

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
	else if (c == reconnectControllers)
	{
		stopThread(20000);
		startThread();
	}
}

void JoyConModule::run()
{	
	connectControllers();

	float rate = jmax(50.0f, 100.0f, JslGetPollRate(0));
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

	controllers.clear();
	JslDisconnectAndDisposeAll();

}
