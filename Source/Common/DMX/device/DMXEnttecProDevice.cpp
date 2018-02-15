/*
  ==============================================================================

	DMXEnttecProDevice.cpp
	Created: 7 Apr 2017 11:23:08am
	Author:  Ben

  ==============================================================================
*/

#include "DMXEnttecProDevice.h"

DMXEnttecProDevice::DMXEnttecProDevice() :
	DMXSerialDevice("DMX Pro",ENTTEC_DMXPRO)
{
}

DMXEnttecProDevice::~DMXEnttecProDevice()
{
}

void DMXEnttecProDevice::setPortConfig()
{
	dmxPort->port->setBaudrate(38400);
	dmxPort->port->setBytesize(serial::eightbits);
	dmxPort->port->setStopbits(serial::stopbits_one);
	dmxPort->port->setParity(serial::parity_none);
	//dmxPort->port->setFlowcontrol(serial::flowcontrol_none);
	//dmxPort->port->setRTS(false);
	dmxPort->port->flush();

}

void DMXEnttecProDevice::sendDMXData()
{
	DBG("Send DMX Data " << (int)sendHeaderData[0] << ", " << (int)sendHeaderData[1]);
	dmxPort->port->write(sendHeaderData, 5);
	dmxPort->port->write(dmxDataOut, 512);
	dmxPort->port->write(sendFooterData, 1);
}

