/*
  ==============================================================================

    DMXOpenUSBDevice.cpp
    Created: 7 Apr 2017 11:22:57am
    Author:  Ben

  ==============================================================================
*/

#include "DMXOpenUSBDevice.h"

DMXOpenUSBDevice::DMXOpenUSBDevice() :
	DMXDevice(OPENDMX),
	Thread("openDMXRead"),
	dmxPort(nullptr)
{
	startThread();

	portParam = new SerialDeviceParameter("Port", "USB Port for the DMX device",true);
	addParameter(portParam);
}

DMXOpenUSBDevice::~DMXOpenUSBDevice()
{
	stopThread(10000);
	setCurrentPort(nullptr);
}

void DMXOpenUSBDevice::setCurrentPort(SerialDevice * port)
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
		dmxPort->open();
		if (!port->isOpen())
		{
			NLOG(niceName, "Could not open port : " << port->info->port);
		}

		lastOpenedPortID = port->info->port;

		dmxPort->port->setBaudrate(250000);
		dmxPort->port->setBytesize(serial::eightbits);
		dmxPort->port->setStopbits(serial::stopbits_two);
		dmxPort->port->setParity(serial::parity_none);
		dmxPort->port->setFlowcontrol(serial::flowcontrol_none);
		dmxPort->port->setRTS(false);
		dmxPort->port->flush();

		dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceConnected);
	}
	else
	{
		dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceDisconnected);
	}
}


void DMXOpenUSBDevice::run()
{
	uint8 startCode[1]{ 0 };
	while (!threadShouldExit())
	{
		if (dmxPort != nullptr && dmxPort->port->isOpen())
		{
			//DBG("send " << (int)dmxDataOut[0] << " / " << (int)dmxDataOut[1] << " / " << (int)dmxDataOut[2]);

			dmxPort->port->setBreak(true);
			dmxPort->port->setBreak(false);
			try
			{
				dmxPort->port->write(startCode, 1); //start code
				dmxPort->port->write(dmxDataOut, 512);
			}
			catch (serial::IOException e)
			{
				DBG("IO Exception on OpenDMX " << e.what());
			}
			catch (serial::PortNotOpenedException)
			{
				DBG("Port Not Opend on OpenDMX");
			}
			catch (serial::SerialException e)
			{
				DBG("Serial Exception : " << e.what());
			}
			
		}

		sleep(23);
	}
	DBG("Exit DMX write thread");
}

void DMXOpenUSBDevice::onContainerParameterChanged(Parameter * p)
{
	if (p == portParam)
	{
		setCurrentPort(portParam->getDevice());
	}
}

void DMXOpenUSBDevice::portAdded(SerialDeviceInfo * info)
{
	if (dmxPort == nullptr && lastOpenedPortID == info->port)
	{
		setCurrentPort(SerialManager::getInstance()->getPort(info));
	}
}

void DMXOpenUSBDevice::portRemoved(SerialDeviceInfo * /*info*/)
{
	setCurrentPort(nullptr);
}

void DMXOpenUSBDevice::portOpened(SerialDevice *)
{
}

void DMXOpenUSBDevice::portClosed(SerialDevice *)
{
}

void DMXOpenUSBDevice::portRemoved(SerialDevice *)
{
	setCurrentPort(nullptr);
}

void DMXOpenUSBDevice::serialDataReceived(const var &)
{
}
