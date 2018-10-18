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

DMXDevice::DMXDevice(const String &name, Type _type, bool canReceive) :
	ControllableContainer(name),
	type(_type),
	canReceive(canReceive)
{
	DMXManager::getInstance()->addDMXManagerListener(this);
	

	memset(dmxDataOut, 0, 512 * sizeof(uint8));
	memset(dmxDataIn, 0, 512 * sizeof(uint8));
}

DMXDevice::~DMXDevice()
{
	if (DMXManager::getInstanceWithoutCreating() != nullptr) DMXManager::getInstance()->removeDMXManagerListener(this);
}

void DMXDevice::sendDMXValue(int channel, int value) //channel 1-512
{
	if (channel > 512) return;
	dmxDataOut[channel-1] = (uint8)value;
}

void DMXDevice::setDMXValueIn(int channel, int value) //channel 1-512
{
	if (channel > 512) return;
	if(dmxDataIn[channel-1] == value) return;
	
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

	case ARTNET:
		return new DMXArtNetDevice();
		break;

	default:
		DBG("Not handled");
		break;
	}

	return nullptr;
}
