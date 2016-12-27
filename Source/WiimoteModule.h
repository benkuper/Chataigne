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

	BoolParameter * connected;
	FloatParameter * pitch;
	FloatParameter * roll;
	Array<BoolParameter *> buttons;
	Wiimote * device;

	void deviceConnected(Wiimote *) override;
	void deviceDisconnected(Wiimote *) override;

	void wiimoteButtonPressed(Wiimote *, Wiimote::WiimoteButton b) override;
	void wiimoteButtonReleased(Wiimote *, Wiimote::WiimoteButton b) override;
	void wiimoteOrientationUpdated(Wiimote *) override;

	static WiimoteModule * create() { return new WiimoteModule(); }
	virtual String getTypeString() const override { return "Wiimote"; }

};


#endif  // WIIMOTEMODULE_H_INCLUDED
