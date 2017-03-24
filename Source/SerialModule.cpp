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
	DBG("Set current port " << (int)port);
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
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Message received :\n" + message);
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