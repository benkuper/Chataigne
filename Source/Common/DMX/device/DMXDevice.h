/*
  ==============================================================================

    DMXDevice.h
    Created: 7 Apr 2017 11:22:47am
    Author:  Ben

  ==============================================================================
*/
#pragma once

class DMXDevice :
	public ControllableContainer,
	public DMXManager::DMXManagerListener,
	public HighResolutionTimer
{
public:
	enum Type { OPENDMX, ENTTEC_DMXPRO, ENTTEC_MK2, ARTNET};
	DMXDevice(const String &name, Type type, bool canReceive);
	virtual ~DMXDevice();

	Type type;

	bool enabled;
	bool isConnected;
	
	uint8 dmxDataOut[512];
	uint8 dmxDataIn[512];
	bool canReceive;

	EnablingControllableContainer* inputCC;

	EnablingControllableContainer* outputCC;
	BoolParameter * alwaysSend;
	IntParameter * targetRate;

	void setConnected(bool value);

	virtual void sendDMXValue(int channel, int value);
	virtual void sendDMXRange(int startChannel, Array<int> values);
	virtual void sendDMXValues();
	virtual void sendDMXValuesInternal() = 0;

	void setDMXValuesIn(int numChannels, uint8* values);

	virtual void clearDevice();
	
	static DMXDevice * create(Type type);

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	virtual void hiResTimerCallback() override;

	class DMXDeviceListener
	{
	public:
		virtual ~DMXDeviceListener() {}

		virtual void dmxDeviceConnected() {}
		virtual void dmxDeviceDisconnected() {}
		virtual void dmxDataInChanged(int /*numChannels*/, uint8* /*values*/) {}
	};

	ListenerList<DMXDeviceListener> dmxDeviceListeners;
	void addDMXDeviceListener(DMXDeviceListener* newListener) { dmxDeviceListeners.add(newListener); }
	void removeDMXDeviceListener(DMXDeviceListener* listener) { dmxDeviceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DMXDevice)

};