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
	public DMXManager::DMXManagerListener
{
public:
	enum Type { OPENDMX, ENTTEC_DMXPRO, ENTTEC_MK2, ARTNET, SACN };
	DMXDevice(const String& name, Type type, bool canReceive);
	virtual ~DMXDevice();

	Type type;

	bool enabled;
	bool isConnected;

	CriticalSection dmxLock;

	bool canReceive;

	EnablingControllableContainer* inputCC;
	EnablingControllableContainer* outputCC;

	void setConnected(bool value);
	virtual void refreshEnabled() {};

	//virtual void sendDMXValue(int net, int subnet, int universe, int channel, int value);
	//virtual void sendDMXRange(int net, int subnet, int universe, int startChannel, Array<int> values);
	virtual void sendDMXValues(DMXUniverse* u);
	virtual void sendDMXValuesInternal(DMXUniverse* u) = 0;

	void setDMXValuesIn(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "");

	virtual void clearDevice();

	static DMXDevice* create(Type type);

	class DMXDeviceListener
	{
	public:
		virtual ~DMXDeviceListener() {}

		virtual void dmxDeviceConnected() {}
		virtual void dmxDeviceDisconnected() {}
		virtual void dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") {}
	};

	ListenerList<DMXDeviceListener> dmxDeviceListeners;
	void addDMXDeviceListener(DMXDeviceListener* newListener) { dmxDeviceListeners.add(newListener); }
	void removeDMXDeviceListener(DMXDeviceListener* listener) { dmxDeviceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DMXDevice)

};