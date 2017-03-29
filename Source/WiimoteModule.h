/*
  ==============================================================================

    WiimoteModule.h
    Created: 26 Dec 2016 6:20:01pm
    Author:  Ben

  ==============================================================================
*/


#ifndef WIIMOTEMODULE_H_INCLUDED
#define WIIMOTEMODULE_H_INCLUDED

#include "Module.h"
#include "WiimoteManager.h"


class WiimoteModule :
	public Module,
	public WiimoteManager::Listener,
	public Wiimote::Listener
{
public:
	WiimoteModule(const String &name = "Wiimote");
	~WiimoteModule();

	//Control params
	IntParameter * deviceID;
	FloatParameter * batteryLevel;

	//Value params
	BoolParameter * connected;
	BoolParameter * nunchuckConnected;
	FloatParameter * pitch;
	FloatParameter * roll;
	Array<BoolParameter *> buttons;
	Wiimote * device;

	void deviceConnected(Wiimote *) override;
	void deviceDisconnected(Wiimote *) override;

	void wiimoteButtonPressed(Wiimote *, Wiimote::WiimoteButton b) override;
	void wiimoteButtonReleased(Wiimote *, Wiimote::WiimoteButton b) override;
	void wiimoteOrientationUpdated(Wiimote *) override;
	void wiimoteBatteryLevelChanged(Wiimote *) override;
	void wiimoteNunchuckPlugged(Wiimote *) override;
	void wiimoteNunchuckUnplugged(Wiimote *) override;

	static WiimoteModule * create() { return new WiimoteModule(); }
	virtual String getDefaultTypeString() const override { return "Wiimote"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WiimoteModule)

};


#endif  // WIIMOTEMODULE_H_INCLUDED
