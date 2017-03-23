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

	selectedHardwareID = addStringParameter("selectedHardwareID", "Id of the selected hardware", "");
	selectedPort = addStringParameter("selectedPort", "Name of the selected hardware", "");

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
		lastOpenedPortID = port->info->port;

		selectedPort->setValue(port->info->port);
		selectedHardwareID->setValue(port->info->hardwareID);
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
	
	if (p == selectedHardwareID || p == selectedPort)
	{
		SerialDevice * _port = SerialManager::getInstance()->getPort(selectedHardwareID->stringValue(), selectedPort->stringValue(), true);
		if (_port != nullptr)
		{
			setCurrentPort(_port);
		}
	}
};


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
	if (logIncomingData->boolValue()) LOG("Data received :\n"<<data.toString());
		
	inActivityTrigger->trigger();
	processMessage(data.toString());
}


void SerialModule::processMessage(const String & message)
{
	
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