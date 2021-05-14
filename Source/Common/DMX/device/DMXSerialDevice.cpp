#include "DMXSerialDevice.h"
/*
  ==============================================================================

    DMXSerialDevice.cpp
    Created: 10 Apr 2017 10:58:01am
    Author:  Ben

  ==============================================================================
*/

DMXSerialDevice::DMXSerialDevice(const String &name, Type type, bool canReceive) :
	DMXDevice(name, type, canReceive),
	dmxPort(nullptr)
{
	portParam = new SerialDeviceParameter("Port", "USB Port for the DMX device", true);
	addParameter(portParam);
	SerialManager::getInstance()->addSerialManagerListener(this);
	channelsParam = outputCC->addIntParameter("Channel count", "Maximum DMX channel sent", 512, 1, 512);
	changeDMXChannels();
}

DMXSerialDevice::~DMXSerialDevice()
{
	if (SerialManager::getInstanceWithoutCreating() != nullptr)
	{
		SerialManager::getInstance()->removeSerialManagerListener(this);
	}
	
	setCurrentPort(nullptr);
}

bool DMXSerialDevice::setPortStatus(bool status) {
	bool connected = false;
	if (dmxPort != nullptr) {
		connected = dmxPort->isOpen();
		bool moduleEnabled = enabled;
		if (status && !connected && moduleEnabled) { //We want to open the port, it's not already opened and the module is enabled
			dmxPort->setMode(SerialDevice::PortMode::RAW);
			dmxPort->open();

			connected = dmxPort->isOpen();
			if (!connected) {
				NLOG(niceName, "Could not open port : " << dmxPort->info->port);
				dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceDisconnected);
				dmxPort = nullptr; //Avoid crash if SerialPort is busy
			}
			else {
				NLOG(niceName, "Port connected : " << dmxPort->info->port);
				setPortConfig();
			}
		}
		else if ((!moduleEnabled || !status) && connected) { //We want to close the port or the module is disabled, and it's actually opened
			NLOG(niceName, "Port disconnected : " << dmxPort->info->port);
			dmxPort->close();
			connected = dmxPort->isOpen();
		}
	}
	setConnected(connected);
	return connected;
}

void DMXSerialDevice::refreshEnabled() {
	setPortStatus(enabled);
}

void DMXSerialDevice::setCurrentPort(SerialDevice* port)
{
	if (dmxPort == port) return;

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

void DMXSerialDevice::sendDMXValuesInternal()
{
	if (dmxPort != nullptr && dmxPort->port->isOpen())
	{
		try
		{
			sendDMXValuesSerialInternal();
		} catch(std::exception e)
		{
			LOGWARNING("Error sending values to DMX, maybe it has been disconnected ?");
		}
	}
}

void DMXSerialDevice::onContainerParameterChanged(Parameter * p)
{
	DMXDevice::onContainerParameterChanged(p);

	if (p == portParam)
	{
		setPortStatus(false);
		SerialDevice* newDevice = portParam->getDevice();
		SerialDevice* prevPort = dmxPort;
		setCurrentPort(newDevice);

		if (dmxPort == nullptr && prevPort != nullptr)
		{
			DBG("Manually set no ghost port");
			lastOpenedPortID = ""; //forces no ghosting when user chose to manually disable port
		}
	}
	else if (p == channelsParam) {
		dmxChannels = channelsParam->getValue();
		changeDMXChannels();
	}
}

void DMXSerialDevice::portAdded(SerialDeviceInfo * info)
{
	if (dmxPort == nullptr && lastOpenedPortID == info->deviceID)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void DMXSerialDevice::portRemoved(SerialDeviceInfo * p)
{
	if (dmxPort != nullptr && dmxPort->info == p) setCurrentPort(nullptr);
}

void DMXSerialDevice::portOpened(SerialDevice *)
{
}

void DMXSerialDevice::portClosed(SerialDevice *)
{
}

void DMXSerialDevice::portRemoved(SerialDevice *)
{
	setCurrentPort(nullptr);
}

void DMXSerialDevice::serialDataReceived(const var &)
{
}