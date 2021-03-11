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
}

DMXSerialDevice::~DMXSerialDevice()
{
	if (SerialManager::getInstanceWithoutCreating() != nullptr)
	{
		SerialManager::getInstance()->removeSerialManagerListener(this);
	}
	
	setCurrentPort(nullptr);
}

void DMXSerialDevice::setCurrentPort(SerialDevice * port)
{
	if (port == dmxPort) return;

	if (dmxPort != nullptr)
	{
		dmxPort->removeSerialDeviceListener(this);
	}

	dmxPort = port;

	if (dmxPort != nullptr)
	{
		dmxPort->addSerialDeviceListener(this);
		dmxPort->setMode(SerialDevice::PortMode::RAW);
		dmxPort->open();
		if (!port->isOpen())
		{
			NLOG(niceName, "Could not open port : " << port->info->port);
			dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceDisconnected);
		}
		else
		{
			NLOG(niceName, "Port connected : " << port->info->port);
			lastOpenedPortID = port->info->port;
			setPortConfig();
			setConnected(true);
		}
	}
	else
	{
		setConnected(false);
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
		setCurrentPort(portParam->getDevice());
	}
}

void DMXSerialDevice::portAdded(SerialDeviceInfo * info)
{
	if (dmxPort == nullptr && lastOpenedPortID == info->port)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void DMXSerialDevice::portRemoved(SerialDeviceInfo * /*info*/)
{
	setCurrentPort(nullptr);
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