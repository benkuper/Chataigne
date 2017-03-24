/*
  ==============================================================================

    SerialDeviceParameter.cpp
    Created: 15 Mar 2017 10:14:56am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialDeviceParameter.h"

SerialDeviceParameter::SerialDeviceParameter(const String & name, const String & description, bool enabled) :
	EnumParameter(name,description,enabled)
{
	SerialManager::getInstance()->addSerialManagerListener(this);
	updatePortList(); 
}

SerialDeviceParameter::~SerialDeviceParameter()
{
}

SerialDevice * SerialDeviceParameter::getDevice()
{
	return currentDevice;
}

void SerialDeviceParameter::setValueInternal(var &v)
{
	EnumParameter::setValueInternal(v);
	var data = getValueData();
	currentDevice = SerialManager::getInstance()->getPort(data.getProperty("hardwareID","").toString(), data.getProperty("port", "").toString(),true);
	DBG("current device from setValueInternal : " << (int)currentDevice);
}

void SerialDeviceParameter::updatePortList()
{
	clearOptions();
	DBG("num ports :" << SerialManager::getInstance()->portInfos.size());

	for (auto &p : SerialManager::getInstance()->portInfos)
	{
		var v(new DynamicObject());
		DBG("Add option : " << p->port << ":" << p->hardwareID);
		v.getDynamicObject()->setProperty("port", p->port);
		v.getDynamicObject()->setProperty("hardwareID", p->hardwareID);
		String desc = p->description;
#if !JUCE_WINDOWS 
		desc += " (" + p->port + ")";
#endif
		addOption(desc, v, false);
	}
}


void SerialDeviceParameter::portAdded(SerialDeviceInfo *)
{
	DBG("param : port added");
	updatePortList();
}

void SerialDeviceParameter::portRemoved(SerialDeviceInfo *)
{
	DBG("param : port removed !");
	updatePortList();
}
