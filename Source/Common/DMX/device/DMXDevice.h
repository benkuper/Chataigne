/*
  ==============================================================================

    DMXDevice.h
    Created: 7 Apr 2017 11:22:47am
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXDEVICE_H_INCLUDED
#define DMXDEVICE_H_INCLUDED

#include "../DMXManager.h"

class DMXDevice :
	public ControllableContainer,
	public DMXManager::DMXManagerListener,
	public Timer
{
public:
	enum Type { OPENDMX, ENTTEC_DMXPRO, ENTTEC_MK2, ARTNET};
	DMXDevice(const String &name, Type type, bool canReceive);
	virtual ~DMXDevice();

	Type type;
	
	uint8 dmxDataOut[512];
	uint8 dmxDataIn[512];
	bool canReceive;

	BoolParameter * fixedRate;
	IntParameter * targetRate;

	virtual void sendDMXValue(int channel, int value);
	void setDMXValueIn(int channel, int value);

	virtual void sendDMXValues() = 0;
	
	static DMXDevice * create(Type type);


	void onContainerParameterChanged(Parameter *p) override;

	class DMXDeviceListener
	{
	public:
		virtual ~DMXDeviceListener() {}

		virtual void dmxDeviceConnected() {}
		virtual void dmxDeviceDisconnected() {}
		virtual void dmxDataInChanged(int /*channel*/, int /*value*/) {}
	};

	ListenerList<DMXDeviceListener> dmxDeviceListeners;
	void addDMXDeviceListener(DMXDeviceListener* newListener) { dmxDeviceListeners.add(newListener); }
	void removeDMXDeviceListener(DMXDeviceListener* listener) { dmxDeviceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DMXDevice)

		// Inherited via Timer
		virtual void timerCallback() override;
};



#endif  // DMXDEVICE_H_INCLUDED
