/*
  ==============================================================================

    DMXOpenUSBDevice.cpp
    Created: 7 Apr 2017 11:22:57am
    Author:  Ben

  ==============================================================================
*/

#if JUCE_LINUX
	#include <sys/ioctl.h>
	#include <asm/termbits.h>
#endif

DMXOpenUSBDevice::DMXOpenUSBDevice() :
	DMXSerialDevice("OpenDMX", OPENDMX, false)
{
}

DMXOpenUSBDevice::~DMXOpenUSBDevice()
{
}

void DMXOpenUSBDevice::setPortConfig()
{
	try
	{

		dmxPort->port->setBaudrate(250000);
		dmxPort->port->setBytesize(serial::eightbits);
		dmxPort->port->setStopbits(serial::stopbits_two);
		dmxPort->port->setParity(serial::parity_none);
		dmxPort->port->setFlowcontrol(serial::flowcontrol_none);
		dmxPort->port->setRTS(false);
		dmxPort->port->setDTR(false);
		dmxPort->port->flush();

#if JUCE_LINUX
#if defined(TCGETS2)
		int fd = dmxPort->port->getHandle();
		static const int rate = 250000;

		struct termios2 tio;  // linux-specific terminal stuff

		if (ioctl(fd, TCGETS2, &tio) < 0)
		{
			DBG("OpenDMX Error in getting termios2 data");
			return;
		}

		tio.c_cflag &= ~CBAUD;
		tio.c_cflag |= BOTHER;
		tio.c_ispeed = 250000;
		tio.c_ospeed = 250000;  // set custom speed directly
		if (ioctl(fd, TCSETS2, &tio) < 0)
		{
			DBG("[UARTWidget] Error in setting termios2 data");
			return;
		}
#endif
#endif
	}
	catch (serial::IOException e)
	{
		LOGERROR("Error opening OpenDMX : " << e.what());
	}

}

void DMXOpenUSBDevice::sendDMXValuesSerialInternal()
{
	try
	{
		dmxPort->port->setBreak(true);
	    dmxPort->port->setBreak(false);
	    dmxPort->port->write(startCode, 1); //start code
		dmxPort->port->write(dmxDataOut, dmxChannels);
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
