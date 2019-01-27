/*
  ==============================================================================

    SerialDeviceParameter.cpp
    Created: 15 Mar 2017 10:14:56am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialDeviceParameter.h"

SerialDeviceParameter::SerialDeviceParameter(const String & name, const String & description, bool enabled) :
	EnumParameter(name,description,enabled),
	currentDevice(nullptr),
	openBaudRate(9600),
	vidFilter(-1),
	pidFilter(-1)
{
	SerialManager::getInstance()->addSerialManagerListener(this);
	updatePortList(); 
}

SerialDeviceParameter::~SerialDeviceParameter()
{
	if(SerialManager::getInstanceWithoutCreating()) SerialManager::getInstance()->removeSerialManagerListener(this);
}

SerialDevice * SerialDeviceParameter::getDevice()
{
	return currentDevice;
}

void SerialDeviceParameter::setValueInternal(var &v)
{
	EnumParameter::setValueInternal(v);
	var data = getValueData();
	if (data.isVoid()) currentDevice = nullptr;
	else currentDevice = SerialManager::getInstance()->getPort(data.getProperty("deviceID","").toString(), data.getProperty("port", "").toString(),true,openBaudRate);
	//DBG("current device from setValueInternal : " << (int)currentDevice);
}

void SerialDeviceParameter::updatePortList()
{
	clearOptions();
	//DBG("num ports :" << SerialManager::getInstance()->portInfos.size());

	if (SerialManager::getInstance()->portInfos.size() > 0) addOption("Not connected or disconnected", var(), false);
	for (auto &p : SerialManager::getInstance()->portInfos)
	{
		if (vidFilter != -1 && p->vid != vidFilter) continue;
		if (pidFilter != -1 && p->pid != pidFilter) continue;

		var v(new DynamicObject());
		//DBG("Add option : " << p->port << ":" << p->hardwareID);
		v.getDynamicObject()->setProperty("port", p->port);
		v.getDynamicObject()->setProperty("deviceID", p->deviceID);
		String desc = p->uniqueDescription;
		addOption(desc, v, false);
	}
}


void SerialDeviceParameter::portAdded(SerialDeviceInfo *)
{
	//DBG("param : port added");
	updatePortList();
}

void SerialDeviceParameter::portRemoved(SerialDeviceInfo *)
{
	//DBG("param : port removed !");
	updatePortList();
}
