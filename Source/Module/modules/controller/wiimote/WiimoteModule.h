/*
  ==============================================================================

    WiimoteModule.h
    Created: 26 Dec 2016 6:20:01pm
    Author:  Ben

  ==============================================================================
*/


#pragma once

class WiimoteModule :
	public Module,
	private Timer
{
public:
	WiimoteModule(const String &name = "Wiimote");
	~WiimoteModule();


	//Control params
	IntParameter * deviceID;
	Trigger * connectTrigger;
	FloatParameter * smoothing;

	//Value params
	BoolParameter * connected;
	BoolParameter * nunchuckConnected;
	BoolParameter * motionPlusConnected;
	FloatParameter * pitch;
	FloatParameter * roll;
	FloatParameter * yaw;
	FloatParameter * nunchuckX;
	FloatParameter * nunchuckY;
	FloatParameter * accelX;
	FloatParameter * accelY;
	FloatParameter * accelZ;
	FloatParameter * rawAccelX;
	FloatParameter * rawAccelY;
	FloatParameter * rawAccelZ;
	FloatParameter * batteryLevel;
	ControllableContainer irCC;
	IntParameter* irNumPoints;
	FloatParameter* irDistance;
	Point3DParameter* irPos;
	Array<Point2DParameter*> irPoints;

	Array<BoolParameter *> buttons;
	void timerCallback() override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static WiimoteModule * create() { return new WiimoteModule(); }
	virtual String getDefaultTypeString() const override { return "Wiimote"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WiimoteModule)

};