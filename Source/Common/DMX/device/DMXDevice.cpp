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
	Thread("DMX Send"),
	type(_type),
    isConnected(false),
    canReceive(canReceive)
{
	DMXManager::getInstance()->addDMXManagerListener(this);

	memset(dmxDataOut, 0, 512 * sizeof(uint8));
	memset(dmxDataIn, 0, 512 * sizeof(uint8));

	fixedRate = addBoolParameter("Use fixed send rate", "If checked, the device will always send the stored values to the constant rate set by the target rate parameter", true);
	targetRate = addIntParameter("Target send rate", "If fixed rate is checked, this is the frequency in Hz of the sending rate", 40, 1, 100);
	sendSleepMS = 1000 / targetRate->intValue();

	startThread();
}

DMXDevice::~DMXDevice()
{
	if (DMXManager::getInstanceWithoutCreating() != nullptr) DMXManager::getInstance()->removeDMXManagerListener(this);

}

void DMXDevice::setConnected(bool value)
{
	if (isConnected == value) return;
	isConnected = value;

	if (isConnected)
	{
		dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceConnected);
		if(fixedRate->boolValue()) startThread();
	} else
	{
		dmxDeviceListeners.call(&DMXDeviceListener::dmxDeviceDisconnected);
		if (isThreadRunning())
		{
			signalThreadShouldExit();
			waitForThreadToExit(1500);
		}
	}
}

void DMXDevice::sendDMXValue(int channel, int value) //channel 1-512
{
	if (channel < 0 || channel > 512) return;
	dmxDataOut[channel-1] = (uint8)value;
	if (!fixedRate->boolValue()) sendDMXValues();
}

void DMXDevice::sendDMXRange(int startChannel, Array<int> values)
{
	int numValues = values.size();
	for (int i = 0; i < numValues; i++)
	{
		int channel = startChannel + i;
		if (channel < 0) continue;
		if (channel > 512) break;

		dmxDataOut[channel - 1] = (uint8)values[i];
	}

	if (!fixedRate->boolValue()) sendDMXValues();
	
}

void DMXDevice::setDMXValueIn(int channel, int value) //channel 1-512
{
	if (channel > 512) return;
	if(dmxDataIn[channel-1] == value) return;
	
	dmxDataIn[channel-1] = (uint8)value;
	//notify
	dmxDeviceListeners.call(&DMXDeviceListener::dmxDataInChanged, channel, value);
}

void DMXDevice::sendDMXValues()
{
}

void DMXDevice::clearDevice()
{
	signalThreadShouldExit();
	waitForThreadToExit(1000);
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

void DMXDevice::onContainerParameterChanged(Parameter * p)
{
	ControllableContainer::onContainerParameterChanged(p);

	if (p == fixedRate || p == targetRate)
	{
		sendSleepMS = 1000 / targetRate->intValue();

		if (p == fixedRate) targetRate->setEnabled(fixedRate->boolValue());
		
		if (fixedRate->boolValue())
		{
			if(!isThreadRunning()) startThread(targetRate->intValue());
		} else
		{
			signalThreadShouldExit();
			waitForThreadToExit(2000);
		}
	}
}

void DMXDevice::run()
{
	while (!threadShouldExit())
	{
		sendDMXValues();
		sleep(sendSleepMS);
	}

	DBG("Exiting DMX Send Thread");
}
