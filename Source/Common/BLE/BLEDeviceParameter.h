/*
  ==============================================================================

	BLEDeviceParameter.h
	Created: 1 Feb 2023 10:52:15pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class BLEDeviceParameter :
	public EnumParameter,
	public BLEManager::BLEManagerListener
{
public:
	BLEDeviceParameter(const String& name, const String& description, bool enabled);
	~BLEDeviceParameter();

	BLEDevice* currentDevice;
	BLEDevice* getDevice();

	String ghostData;
	StringArray nameFilters;

	void setValueInternal(var& value) override;

	void updatePortList();
	
	virtual void bleDeviceAdded(BLEDevice* device) override;
	virtual void bleDeviceRemoved(BLEDevice* device) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BLEDeviceParameter)
};