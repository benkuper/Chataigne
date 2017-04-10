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
#include "DMXArtNetDevice.h"

DMXDevice::DMXDevice(Type _type) :
	ControllableContainer("Device"),
	type(_type)
{
	DMXManager::getInstance()->addDMXManagerListener(this);

	memset(dmxDataOut, 0, 512 * sizeof(uint8));
	memset(dmxDataIn, 0, 512 * sizeof(uint8));
}

DMXDevice::~DMXDevice()
{
	if (DMXManager::getInstanceWithoutCreating() != nullptr) DMXManager::getInstance()->removeDMXManagerListener(this);
}

void DMXDevice::sendDMXValue(int channel, int value) //channel 0-511
{
	dmxDataOut[channel] = (uint8)value;
}

void DMXDevice::setDMXValueIn(int channel, int value) //channel 0-511
{
	if (dmxDataIn[channel] == value) return;
	dmxDataIn[channel] = (uint8)value;
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

	case ARTNET:
		return new DMXArtNetDevice();
		break;

	default:
		DBG("Not handled");
		break;
	}

	return nullptr;
}
