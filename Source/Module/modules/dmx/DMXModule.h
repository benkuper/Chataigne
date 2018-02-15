/*
  ==============================================================================

    DMXModule.h
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXMODULE_H_INCLUDED
#define DMXMODULE_H_INCLUDED

#include "Module/Module.h"
#include "Common/DMX/device/DMXDevice.h"

class DMXModule :
	public Module,
	public DMXDevice::DMXDeviceListener
{
public:
	DMXModule();
	~DMXModule();

	Array<IntParameter *> dmxInValues;

	EnumParameter * dmxType;
	ScopedPointer<DMXDevice> dmxDevice;
	BoolParameter * dmxConnected;

	void setCurrentDMXDevice(DMXDevice * d);

	void sendDMXValue(int channel, int value);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void dmxDeviceConnected() override;
	void dmxDeviceDisconnected() override;

	void dmxDataInChanged(int channel, int value) override;

	static DMXModule * create() { return new DMXModule(); }
	virtual String getDefaultTypeString() const override { return "DMX"; }


	class DMXModuleListener
	{
	public:
		virtual ~DMXModuleListener() {}

		virtual void dmxDeviceChanged() {}
	};

	ListenerList<DMXModuleListener> dmxModuleListeners;
	void addDMXModuleListener(DMXModuleListener* newListener) { dmxModuleListeners.add(newListener); }
	void removeDMXModuleListener(DMXModuleListener* listener) { dmxModuleListeners.remove(listener); }
};


#endif  // DMXMODULE_H_INCLUDED
