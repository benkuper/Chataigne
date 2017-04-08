/*
  ==============================================================================

    DMXDevice.cpp
    Created: 7 Apr 2017 11:22:47am
    Author:  Ben

  ==============================================================================
*/

#include "DMXDevice.h"

#include "DMXOpenUSBDevice.h"
#include "DMXEnttecProDevice.h"

DMXDevice::DMXDevice(Type _type) :
	ControllableContainer("Device"),
	type(_type)
{
	memset(dmxDataOut, 0, 512 * sizeof(uint8));
	memset(dmxDataIn, 0, 512 * sizeof(uint8));
}

DMXDevice::~DMXDevice()
{
}

void DMXDevice::sendDMXValue(int channel, int value) //channel 1-512
{
	dmxDataOut[channel - 1] = (uint8)value;
}

void DMXDevice::setDMXValueIn(int channel, int value) //channel 1-512
{
	if (dmxDataIn[channel-1] == value) return;
	dmxDataIn[channel-1] = (uint8)value;
	//notify
	dmxDeviceListeners.call(&DMXDeviceListener::dmxDataInChanged, channel, value);
}

DMXDevice * DMXDevice::create(Type type)
{
	switch (type)
	{
	case OPENDMX:
		return new DMXOpenUSBDevice();
		break;

	case ENTTEC_DMXPRO:
		return new DMXEnttecProDevice();
		break;

	default:
		DBG("Not handled");
		break;
	}

	return nullptr;
}
