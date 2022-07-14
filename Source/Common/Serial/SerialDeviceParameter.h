/*
  ==============================================================================

    SerialDeviceParameter.h
    Created: 15 Mar 2017 10:14:56am
    Author:  Ben-Portable

  ==============================================================================
*/

#pragma once

class SerialDeviceParameter :
	public EnumParameter,
	public SerialManager::SerialManagerListener
{
public:
	SerialDeviceParameter(const String &name, const String &description, bool enabled);
	~SerialDeviceParameter();

	SerialDevice * currentDevice;
	SerialDevice * getDevice();

	String ghostData;
	int openBaudRate;
	Array<int> vidFilters;
	Array<int> pidFilters;

	void setValueInternal(var &value) override;

	void updatePortList();

	// Inherited via SerialManagerListener
	virtual void portAdded(SerialDeviceInfo * info) override;
	virtual void portRemoved(SerialDeviceInfo * info) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SerialDeviceParameter)
};