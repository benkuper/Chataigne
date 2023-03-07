/*
  ==============================================================================

	BLEDeviceParameter.cpp
	Created: 1 Feb 2023 10:52:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

BLEDeviceParameter::BLEDeviceParameter(const String& name, const String& description, bool enabled) :
	EnumParameter(name, description, enabled),
	currentDevice(nullptr)
{
	BLEManager::getInstance()->addBLEManagerListener(this);
	updatePortList();
}

BLEDeviceParameter::~BLEDeviceParameter()
{
	if (BLEManager::getInstanceWithoutCreating()) BLEManager::getInstance()->removeBLEManagerListener(this);
}

BLEDevice* BLEDeviceParameter::getDevice()
{
	return currentDevice;
}

void BLEDeviceParameter::setValueInternal(var& v)
{
	EnumParameter::setValueInternal(v);
	var data = getValueData();
	if (data.isVoid()) currentDevice = nullptr;
	else currentDevice = BLEManager::getInstance()->getDevice(data.getProperty("deviceID", "").toString());
	//DBG("current device from setValueInternal : " << (int)currentDevice);
}

void BLEDeviceParameter::updatePortList()
{
	//DBG("num ports :" << BLEManager::getInstance()->portInfos.size());

	String s;
	if (currentDevice != nullptr)
	{
		s = currentDevice->peripheral.identifier();
	}

	//clearOptions();
	enumValues.clear();

	if (BLEManager::getInstance()->devices.size() > 0) addOption("Not connected or disconnected", var(), false);
	for (auto& d : BLEManager::getInstance()->devices)
	{
		if (!nameFilters.isEmpty())
		{
			for (auto& n : nameFilters) if (!d->id.contains(n)) continue;
		}

		var v(new DynamicObject());
		v.getDynamicObject()->setProperty("deviceID", d->id);
		addOption(d->description, v, false);
	}

	if (s.isNotEmpty()) setValueWithKey(s);

}


void BLEDeviceParameter::bleDeviceAdded(BLEDevice* s)
{
	//DBG("param : device added");
	updatePortList();
	if (getValueKey() == "" && ghostData == s->id)
	{
		setValueWithKey(ghostData);
		ghostData = "";
	}
}

void BLEDeviceParameter::bleDeviceRemoved(BLEDevice* s)
{
	//DBG("param : device removed !");
	if (s->id == getValueKey())
	{
		ghostData = s->id;
		setValue("");
	}

	updatePortList();
}
