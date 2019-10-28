/*
  ==============================================================================

    SerialManager.h
    Created: 15 Mar 2017 10:14:26am
    Author:  Ben-Portable

  ==============================================================================
*/

#pragma once

#include "SerialDevice.h"

class SerialManager :
	public Timer
{
public:
	juce_DeclareSingleton(SerialManager, true);

	SerialManager();
	virtual ~SerialManager();

	OwnedArray<SerialDeviceInfo> portInfos;
	OwnedArray<SerialDevice> openedPorts;

	void updateDeviceList();

	SerialDevice * getPort(SerialDeviceInfo *  portInfo, bool createIfNotThere = true, int openBaudRate = 9600);
	SerialDevice * getPort(String deviceID, String portName, bool createIfNotThere = true, int openBaudRate = 9600);

	void removePort(SerialDevice * p);

	class SerialManagerListener
	{
	public:
		virtual ~SerialManagerListener() {}
		virtual void portAdded(SerialDeviceInfo  *info) = 0;
		virtual void portRemoved(SerialDeviceInfo  *info) = 0;
	};

	ListenerList<SerialManagerListener> listeners;
	void addSerialManagerListener(SerialManagerListener* newListener) { listeners.add(newListener); }
	void removeSerialManagerListener(SerialManagerListener* listener) { listeners.remove(listener); }


	// Inherited via Timer
	virtual void timerCallback() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SerialManager)
};