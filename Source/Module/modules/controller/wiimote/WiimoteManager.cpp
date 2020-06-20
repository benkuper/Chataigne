/*
  ==============================================================================

    WiimoteManager.cpp
    Created: 27 Dec 2016 1:15:35am
    Author:  Ben

  ==============================================================================
*/


#include "WiimoteManager.h"
#include "WiimotePairUtil.h"


juce_ImplementSingleton(WiimoteManager)

WiimoteManager::WiimoteManager() :
	Thread("wiimoteManager"),
    wiiuseIsInit(false),
    reinitWiimotes(false)
{
	startThread();
}

WiimoteManager::~WiimoteManager()
{
	WiiPairUtil::shouldStop = true;
	signalThreadShouldExit();
	while (isThreadRunning());
	wiiuse_cleanup(devices, MAX_WIIMOTES);

}

void WiimoteManager::addWiimote(wiimote_t * device)
{
	Wiimote * w = new Wiimote(wiimotes.size(),device);
	wiimotes.add(w);
	listeners.call(&Listener::deviceConnected, w);
}

void WiimoteManager::removeWiimote(Wiimote * w)
{
	wiimotes.removeObject(w, false);
	listeners.call(&Listener::deviceDisconnected, w);
	delete w;
}

void WiimoteManager::reconnect(bool autoPairIfNotFound)
{
	if (wiiuseIsInit)
	{
		wiiuse_cleanup(devices, MAX_WIIMOTES);
	}

	while (wiimotes.size() > 0) removeWiimote(wiimotes[0]);

	int numFound = wiiuse_find(devices, MAX_WIIMOTES, 1);
	NLOG("Wiimote", "Found " << numFound << " wiimotes");

	if (threadShouldExit()) return;

	int numConnected = wiiuse_connect(devices, MAX_WIIMOTES);
	NLOG("Wiimote", "Connected to " << numConnected << " wiimotes");

	if (threadShouldExit()) return;

	//only support for windows for now
#if JUCE_WINDOWS
	if (wiimotes.size() == 0 && autoPairIfNotFound)
	{
		NLOG("Wiimote", "No wiimote found, trying auto-pairing..");
		int  pairResult = WiiPairUtil::pair();
		NLOG("Wiimote", pairResult << " devices paired.");

		if (threadShouldExit()) return;

		reconnect(false);
		return;
	}
#endif

	for (int i = 0; i < numConnected; ++i)
	{
		addWiimote(devices[i]);
	}


	reinitWiimotes = false;
}

void WiimoteManager::run()
{
	devices = wiiuse_init(MAX_WIIMOTES);
	
	reconnect(true);


	int i = 0;
	numReconnectTries = 0;
	while (!threadShouldExit())
	{
		
		if (reinitWiimotes)
		{
			reconnect(true);
		}

		if (wiiuse_poll(devices, MAX_WIIMOTES)) {
			for (auto &w : wiimotes)
			{
				w->update();
			}

			i = 0;
		} else
		{
			i++;
		}

		if (i > 200 && numReconnectTries < 2) //every 5s
		{
			numReconnectTries++;
			reinitWiimotes = true;
			i = 0;
		}
		sleep(10);
	}
}

Wiimote::Wiimote(int _id, wiimote_t * _device) :
	id(_id),
	smoothing(.5f)
{
	setDevice(_device);
}

Wiimote::~Wiimote()
{
}



void Wiimote::setDevice(wiimote_t * _device)
{
	device = _device;
	
	wiiuse_motion_sensing(device, 1);
	wiiuse_set_flags(device, WIIUSE_SMOOTHING, 0);
	wiiuse_set_smooth_alpha(device, smoothing);

	switch (id)
	{
	case 0: wiiuse_set_leds(device, WIIMOTE_LED_1); break;
	case 1: wiiuse_set_leds(device, WIIMOTE_LED_2); break;
	case 2: wiiuse_set_leds(device, WIIMOTE_LED_3); break;
	case 3: wiiuse_set_leds(device, WIIMOTE_LED_4); break;
	}
}

void Wiimote::setSmoothing(float value)
{
	smoothing = value;
	wiiuse_set_smooth_alpha(device, smoothing);

}

bool Wiimote::isButtonDown(WiimoteButton b)
{
	return buttons[(int)b];
}

void Wiimote::update()
{
	setConnected(WIIMOTE_IS_CONNECTED(device));
	setBatteryLevel(device->battery_level);
	//DBG("Exp : " << (int)device->expansion_state);

	switch (device->event)
	{
	case WIIUSE_EVENT_TYPE::WIIUSE_EVENT:
		for (int i = 0; i < NUM_WIIMOTE_BUTTONS; ++i) setButton(i, device->btns >> i & 1);

		setAccel(device->gforce.x,device->gforce.y,device->gforce.z,device->accel.x, device->accel.y, device->accel.z);
		setYPR(device->orient.yaw/180, -device->orient.pitch/90, device->orient.roll/180);
		setNunchuckXY(device->exp.nunchuk.js.x, device->exp.nunchuk.js.y);
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_CONNECT:
		NLOG("Wiimote", "Wiimote disconnected");
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_DISCONNECT:
	case WIIUSE_EVENT_TYPE::WIIUSE_UNEXPECTED_DISCONNECT:
		NLOG("Wiimote","Wiimote disconnected");
		//WiimoteManager::getInstance()->reloadWiimotes = true;
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_NUNCHUK_INSERTED:
		NLOG("Wiimote", "Nunchuck plugged");
		listeners.call(&Listener::wiimoteNunchuckPlugged,this);
		break;

		
	case WIIUSE_EVENT_TYPE::WIIUSE_NUNCHUK_REMOVED:
		NLOG("Wiimote", "Nunchuck unplugged");
		listeners.call(&Listener::wiimoteNunchuckUnplugged, this);
		break;
		
	case WIIUSE_EVENT_TYPE::WIIUSE_MOTION_PLUS_ACTIVATED:
		NLOG("Wiimote", "Motionplus plugged");
		listeners.call(&Listener::wiimoteMotionPlusPlugged, this);
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_MOTION_PLUS_REMOVED:
		NLOG("Wiimote", "Motionplus unplugged");
		listeners.call(&Listener::wiimoteMotionPlusUnplugged, this);
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_CLASSIC_CTRL_INSERTED:
		NLOG("Wiimote", "Classic plugged");
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_CLASSIC_CTRL_REMOVED:
		NLOG("Wiimote", "Classic unplugged");
		break;

        default:
            break;
	}
}

void Wiimote::setButton(int index, bool value)
{
//	if(value) DBG("Push button " << index);
	if (buttons[index] == value) return;

	buttons[index] = value;
	//DBG("Button # " << index << " = " << (int)value);
	listeners.call(value ? &Listener::wiimoteButtonPressed : &Listener::wiimoteButtonReleased, this, (WiimoteButton)index);
}

void Wiimote::setAccel(float x, float y, float z, float rx, float ry, float rz)
{
	if (accelX == x && accelY == y && accelZ == z) return;
	accelX = x;
	accelY = y;
	accelZ = z;
	rawAccelX = rx;
	rawAccelY = ry;
	rawAccelZ = rz;
	listeners.call(&Listener::wiimoteAccelUpdated, this);
}

void Wiimote::setYPR(float _yaw, float _pitch, float _roll)
{
	if (yaw == _yaw && pitch == _pitch && roll == _roll) return;

	yaw = _yaw;
	pitch = _pitch;
	roll = _roll;
	listeners.call(&Listener::wiimoteOrientationUpdated, this);
}

void Wiimote::setNunchuckXY(float x, float y)
{
	if (joystickX == x && joystickY == y) return;

	joystickX = x;
	joystickY = y;

	listeners.call(&Listener::wiimoteJoystickUpdated, this);
}

void Wiimote::setConnected(bool value)
{
	if (isConnected != value)
	{
		isConnected = value;
		listeners.call(isConnected?&Listener::wiimoteDisconnected:&Listener::wiimoteDisconnected,this);
	}
}

void Wiimote::setBatteryLevel(float value)
{
	if (batteryLevel != value)
	{
		batteryLevel = value;
		listeners.call(&Listener::wiimoteBatteryLevelChanged, this);
	}
}
