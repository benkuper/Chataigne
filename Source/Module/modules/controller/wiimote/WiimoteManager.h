/*
  ==============================================================================

    WiimoteManager.h
    Created: 27 Dec 2016 1:15:35am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "wiiuse.h"

#define MAX_WIIMOTES 2
#define NUM_WIIMOTE_BUTTONS 13


class Wiimote
{
public:
	Wiimote(int id, wiimote_t * device);
	~Wiimote();

	int id;
	wiimote_t * device;

	enum WiimoteButton {TWO,ONE, B, A,MINUS,C,Z,HOME,LEFT,RIGHT,DOWN,UP,PLUS};

	bool isConnected;
	float batteryLevel;

	float smoothing;
	bool buttons[NUM_WIIMOTE_BUTTONS]; //11 one wiimote, 2 on nunchuck
	float yaw;
	float pitch;
	float roll;
	float accelX;
	float accelY;
	float accelZ;
	float rawAccelX;
	float rawAccelY;
	float rawAccelZ;

	//Nunchuck
	float nunchuckAccelX;
	float nunchuckAccelY;
	float nunchuckAccelZ;
	float joystickX;
	float joystickY;

	float irDistance;
	int irNumDots;
	Vector3D<float> irPos;
	Array<Point<float>> irPoints;


	void setDevice(wiimote_t * _device);
	void setSmoothing(float value);

	bool isButtonDown(WiimoteButton);
	void update();

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void wiimoteConnected(Wiimote*) {}
		virtual void wiimoteDisconnected(Wiimote *) {}
		virtual void wiimoteButtonPressed(Wiimote *,WiimoteButton) {}
		virtual void wiimoteButtonReleased(Wiimote *,WiimoteButton) {}
		virtual void wiimoteAccelUpdated(Wiimote *) {}
		virtual void wiimoteOrientationUpdated(Wiimote *) {}
		virtual void wiimoteJoystickUpdated(Wiimote *) {}
		virtual void wiimoteIRPointsUpdated(Wiimote*) {}
		virtual void wiimoteNunchuckPlugged(Wiimote *) {}
		virtual void wiimoteNunchuckUnplugged(Wiimote *) {}
		virtual void wiimoteMotionPlusPlugged(Wiimote *) {}
		virtual void wiimoteMotionPlusUnplugged(Wiimote *) {}
		virtual void wiimoteBatteryLevelChanged(Wiimote*) {}
	};

	ListenerList<Listener> listeners;
	void addListener(Listener* newListener) { listeners.add(newListener); }
	void removeListener(Listener* listener) { listeners.remove(listener); }


private :
	void setButton(int index, bool value);
	void setAccel(float x, float y, float z, float rx, float ry, float rz);
	void setYPR(float yaw, float pitch, float roll);
	void setNunchuckXY(float x, float y);
	void setConnected(bool value);
	void setBatteryLevel(float value);
	void setIRPoints(int numDots, float distance, Vector3D<float> estimatedPos, Array<Point<float>> points);

};

class WiimoteManager :
	public Thread
{
public:
	juce_DeclareSingleton(WiimoteManager, false);
	WiimoteManager();
	~WiimoteManager();

	wiimote ** devices;
	OwnedArray<Wiimote> wiimotes;

	bool wiiuseIsInit;
	bool reinitWiimotes;

	int numReconnectTries;

	void addWiimote(wiimote_t * device);
	void removeWiimote(Wiimote * wiimote);

	void reconnect(bool autoPairIfNotFound);

	// Inherited via Thread
	virtual void run() override;


	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void deviceConnected(Wiimote *) {}
		virtual void deviceDisconnected(Wiimote *) {}
	};

	ListenerList<Listener> listeners;
	void addListener(Listener* newListener) { listeners.add(newListener); }
	void removeListener(Listener* listener) { listeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WiimoteManager)

};