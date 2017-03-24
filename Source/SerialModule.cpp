/*
  ==============================================================================

    SerialModule.cpp
    Created: 15 Mar 2017 10:17:22am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialModule.h"


SerialModule::SerialModule(const String &name) :
	Module(name),
	port(nullptr)
{
	portParam = new SerialDeviceParameter("Port", "Serial Port to connect",true);
	addParameter(portParam);
	
	modeParam = addEnumParameter("Mode", "Protocol for treating the incoming data");
	modeParam->addOption("Lines", SerialDevice::LINES);
	modeParam->addOption("Raw", SerialDevice::RAW);
	modeParam->addOption("Data255", SerialDevice::DATA255);
	modeParam->addOption("COBS", SerialDevice::COBS);
	
	SerialManager::getInstance()->addSerialManagerListener(this);
}

SerialModule::~SerialModule()
{
	if (SerialManager::getInstanceWithoutCreating() != nullptr)
	{
		SerialManager::getInstance()->removeSerialManagerListener(this);
	}

	setCurrentPort(nullptr);
}

void SerialModule::setCurrentPort(SerialDevice * _port)
{
	if (port == _port) return;


	if (port != nullptr)
	{

		port->removeSerialDeviceListener(this);
	}

	port = _port;

	if (port != nullptr)
	{
		port->addSerialDeviceListener(this);
		port->open();
		if (!port->isOpen())
		{
			NLOG(niceName, "Could not open port : " << port->info->port);
		}
		else
		{
			port->mode = (SerialDevice::PortMode)(int)modeParam->getValueData();
		}
		lastOpenedPortID = port->info->port;
	}
	else
	{
		DBG("set port null");
	}

	DBG("current port changed");
	serialModuleListeners.call(&SerialModuleListener::currentPortChanged);
}


void SerialModule::onContainerParameterChangedInternal(Parameter * p) {
	Module::onContainerParameterChangedInternal(p);
	
	DBG("Parameter changed");
	if (p == portParam)
	{
		 setCurrentPort(portParam->getDevice());
	}if (p == modeParam)
	{
		if (port != nullptr) port->mode = (SerialDevice::PortMode)(int)modeParam->getValueData();
	}
};


void SerialModule::processDataLine(const String & message)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Message received :\n" + message);
	}
}

void SerialModule::processData255(Array<uint8_t> data)
{
	DBG("process data 255 :" << (int)data[0]);
}

void SerialModule::processDataRaw(Array<uint8_t> data)
{ 
	
	DBG("process data raw  :" << (int)data[0]);
}

void SerialModule::processDataCOBS(Array<uint8_t> data)
{
	DBG("process data COBS !" << (int)data[0]);
}


void SerialModule::portOpened(SerialDevice *)
{
	serialModuleListeners.call(&SerialModuleListener::portOpened);

}

void SerialModule::portClosed(SerialDevice *)
{
	serialModuleListeners.call(&SerialModuleListener::portClosed);
}

void SerialModule::portRemoved(SerialDevice *)
{
	setCurrentPort(nullptr);
}

void SerialModule::serialDataReceived(const var & data)
{
	if (logIncomingData->boolValue())
	{
		LOG("Data received :\n" << data.toString());
	}
		
	inActivityTrigger->trigger();
	
	switch (port->mode)
	{
	case SerialDevice::LINES:
		processDataLine(data.toString());
		break;

	case SerialDevice::DATA255:
	{
		Array<uint8> bytes((const uint8_t *)data.getBinaryData()->getData(), data.getBinaryData()->getSize());
		processData255(bytes);
	}
		
		break;

	case SerialDevice::RAW: 
	{
		Array<uint8> bytes((const uint8_t *)data.getBinaryData()->getData(), data.getBinaryData()->getSize());
		processDataRaw(bytes);
	}
	break;
	}
	
}

void SerialModule::portAdded(SerialDeviceInfo * info)
{
	//DBG("SerialModule, portAdded >" << info->hardwareID << "< > " << lastOpenedPortID);
	if (port == nullptr && lastOpenedPortID == info->port)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void SerialModule::portRemoved(SerialDeviceInfo *)
{
}