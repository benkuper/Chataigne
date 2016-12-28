/*
  ==============================================================================

    WiimoteManager.cpp
    Created: 27 Dec 2016 1:15:35am
    Author:  Ben

  ==============================================================================
*/


#include "WiimoteManager.h"
#include "DebugHelpers.h"

juce_ImplementSingleton(WiimoteManager)

WiimoteManager::WiimoteManager() :
	Thread("wiimoteManager")
{
	startThread();
}

WiimoteManager::~WiimoteManager()
{
	stopThread(100);
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

void WiimoteManager::run()
{
	wiimote** devices = wiiuse_init(MAX_WIIMOTES);
	int numFound = wiiuse_find(devices, MAX_WIIMOTES, 1);
	int numConnected = wiiuse_connect(devices, MAX_WIIMOTES);
	
	if (!numConnected)
	{
		NLOG("Wiimote", "Failed to connect to any wiimote.\n");
		return;
	}

	NLOG("Wiimote", "Connected to " << numConnected << " wiimotes (of " << numFound << " found)");

	
	for (int i = 0; i < MAX_WIIMOTES; ++i) {
		addWiimote(devices[i]);
	}
	

	while (!threadShouldExit())
	{
		if (wiiuse_poll(devices, MAX_WIIMOTES)) {
			for (auto &w : wiimotes)
			{
				w->update();
			}
		}

		sleep(5);
	}
}

Wiimote::Wiimote(int _id, wiimote_t * _device) :
	id(_id),
	device(_device)
{
	wiiuse_motion_sensing(device, 1);
	wiiuse_set_flags(device, WIIUSE_SMOOTHING, 0);
	switch (id)
	{
	case 0: wiiuse_set_leds(device, WIIMOTE_LED_1); break;
	case 1: wiiuse_set_leds(device, WIIMOTE_LED_2); break;
	case 2: wiiuse_set_leds(device, WIIMOTE_LED_3); break;
	case 3: wiiuse_set_leds(device, WIIMOTE_LED_4); break;
	}
}

Wiimote::~Wiimote()
{
}

bool Wiimote::isButtonDown(WiimoteButton b)
{
	return buttons[(int)b];
}

void Wiimote::update()
{
	setConnected(WIIMOTE_IS_CONNECTED(device));
	setBatteryLevel(device->battery_level);
	DBG("Exp : " << (int)device->expansion_state);

	switch (device->event)
	{
	case WIIUSE_EVENT_TYPE::WIIUSE_EVENT:
		DBG("BT " << (int)device->btns << " / Nunchuck bt : " << (int)device->exp.nunchuk.btns);
		for (int i = 0; i < NUM_WIIMOTE_BUTTONS; i++) setButton(i, device->btns >> i & 1);
		setAccel(device->gforce.x, device->gforce.y, device->gforce.z);
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_CONNECT:
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_DISCONNECT:
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_NUNCHUK_INSERTED:
		DBG("Nunchuk ON");
		listeners.call(&Listener::wiimoteNunchuckPlugged,this);
		break;

		
	case WIIUSE_EVENT_TYPE::WIIUSE_NUNCHUK_REMOVED:
		DBG("Nunchuk OFF");
		listeners.call(&Listener::wiimoteNunchuckUnplugged, this);
		break;
		

	case WIIUSE_EVENT_TYPE::WIIUSE_CLASSIC_CTRL_INSERTED:
		break;

	case WIIUSE_EVENT_TYPE::WIIUSE_CLASSIC_CTRL_REMOVED:
		break;
	}
}

void Wiimote::setButton(int index, bool value)
{
	if (buttons[index] == value) return;

	buttons[index] = value;
	//DBG("Button # " << index << " = " << (int)value);
	listeners.call(value ? &Listener::wiimoteButtonPressed : &Listener::wiimoteButtonReleased, this, (WiimoteButton)index);
}

void Wiimote::setAccel(float x, float y, float z)
{
	if (x == gforceX && y == gforceY && z == gforceZ) return;
	gforceX = x;
	gforceY = y;
	gforceZ = z;
	listeners.call(&Listener::wiimoteOrientationUpdated, this);
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
