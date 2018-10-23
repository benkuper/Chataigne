/*
  ==============================================================================

    DMXOpenUSBDevice.cpp
    Created: 7 Apr 2017 11:22:57am
    Author:  Ben

  ==============================================================================
*/

#include "DMXOpenUSBDevice.h"

DMXOpenUSBDevice::DMXOpenUSBDevice() :
	DMXSerialDevice("OpenDMX", OPENDMX, false)
{
}

DMXOpenUSBDevice::~DMXOpenUSBDevice()
{
}

void DMXOpenUSBDevice::setPortConfig()
{
	dmxPort->port->setBaudrate(250000);
	dmxPort->port->setBytesize(serial::eightbits);
	dmxPort->port->setStopbits(serial::stopbits_two);
	dmxPort->port->setParity(serial::parity_none);
	dmxPort->port->setFlowcontrol(serial::flowcontrol_none);
	dmxPort->port->setRTS(false);
	dmxPort->port->flush();
}

void DMXOpenUSBDevice::sendDMXValuesSerialInternal()
{
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
