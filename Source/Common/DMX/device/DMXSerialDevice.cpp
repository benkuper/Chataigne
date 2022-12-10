/*
  ==============================================================================

	DMXSerialDevice.cpp
	Created: 10 Apr 2017 10:58:01am
	Author:  Ben

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXSerialDevice::DMXSerialDevice(const String& name, Type type, bool canReceive) :
	DMXDevice(name, type, canReceive),
	dmxPort(nullptr)
{
	portParam = new SerialDeviceParameter("Port", "USB Port for the DMX device", true);
	addParameter(portParam);
	SerialManager::getInstance()->addSerialManagerListener(this);
}

DMXSerialDevice::~DMXSerialDevice()
{
	if (SerialManager::getInstanceWithoutCreating() != nullptr)
	{
		SerialManager::getInstance()->removeSerialManagerListener(this);
	}

	setCurrentPort(nullptr);
}

bool DMXSerialDevice::setPortStatus(bool status)
{

	if (dmxPort == nullptr)
	{
		setConnected(false);
		return false;
	}

	bool shouldOpen = status && enabled;

	if (shouldOpen)
	{
		dmxPort->setMode(SerialDevice::PortMode::RAW);
		if (!dmxPort->isOpen()) dmxPort->open();

		if (!dmxPort->isOpen())
		{
			NLOGERROR(niceName, "Could not open port : " << dmxPort->info->port);
			dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceDisconnected);
			dmxPort = nullptr; //Avoid crash if SerialPort is busy
		}
		else
		{
			NLOG(niceName, "Port " << dmxPort->info->port << " opened, let's light up the party !");
			setPortConfig();
		}
	}
	else //We want to close the port or the module is disabled, and it's actually opened
	{
		NLOG(niceName, "Port disconnected : " << dmxPort->info->port);
		if (dmxPort->isOpen()) dmxPort->close();
	}

	setConnected(dmxPort->isOpen());
	return dmxPort->isOpen();
}

void DMXSerialDevice::refreshEnabled()
{
	setPortStatus(enabled);
}

void DMXSerialDevice::setCurrentPort(SerialDevice* port)
{
	if (dmxPort == port) return;

	setPortStatus(false);

	if (dmxPort != nullptr)
	{
		dmxPort->removeSerialDeviceListener(this);
	}

	dmxPort = port;
	setPortStatus(true);

	if (dmxPort != nullptr)
	{
		DBG(" > " << dmxPort->info->port);
		dmxPort->addSerialDeviceListener(this);
		lastOpenedPortID = dmxPort->info->deviceID;
	}
}

void DMXSerialDevice::processIncomingData()
{
	DBG("Incoming data, process function not overriden, doing nothing.");
}

void DMXSerialDevice::sendDMXValuesInternal(DMXUniverse* u)
{
	if (dmxPort == nullptr) return;

	if (dmxPort->port->isOpen())
	{
		try
		{
			sendDMXValuesSerialInternal(u);
		}
		catch (std::exception e)
		{
			LOGWARNING("Error sending values to DMX, maybe it has been disconnected ?");
		}
	}
}

void DMXSerialDevice::onContainerParameterChanged(Parameter* p)
{
	DMXDevice::onContainerParameterChanged(p);

	if (p == portParam)
	{
		SerialDevice* newDevice = portParam->getDevice();
		SerialDevice* prevPort = dmxPort;
		setCurrentPort(newDevice);

		if (dmxPort == nullptr && prevPort != nullptr)
		{
			DBG("Manually set no ghost port");
			lastOpenedPortID = ""; //forces no ghosting when user chose to manually disable port
		}
	}
}

void DMXSerialDevice::portAdded(SerialDeviceInfo* info)
{
	if (dmxPort == nullptr && lastOpenedPortID == info->deviceID)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void DMXSerialDevice::portRemoved(SerialDeviceInfo* p)
{
	if (dmxPort != nullptr && dmxPort->info == p) setCurrentPort(nullptr);
}

void DMXSerialDevice::portOpened(SerialDevice*)
{
}

void DMXSerialDevice::portClosed(SerialDevice*)
{
}

void DMXSerialDevice::portRemoved(SerialDevice*)
{
	setCurrentPort(nullptr);
}

void DMXSerialDevice::serialDataReceived(const var&)
{
}