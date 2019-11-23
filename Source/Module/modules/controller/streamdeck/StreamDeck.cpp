/*
  ==============================================================================

    StreamDeck.cpp
    Created: 23 Nov 2019 2:48:28am
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeck.h"


StreamDeck::StreamDeck(hid_device* device, String serialNumber) :
	Thread("StreamDeck"),
	device(device),
	serialNumber(serialNumber),
	numButtons(15),
	imageSize(72)
{
	hid_set_nonblocking(device, 1);
	for (int i = 0; i < numButtons; i++) buttonStates.add(false);

	startThread();
}

StreamDeck::~StreamDeck()
{
	signalThreadShouldExit();
	waitForThreadToExit(500);
	stopThread(100);
}

void StreamDeck::reset()
{
	const uint8_t resetData[17]{ 0x0B, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	sendFeatureReport(resetData, 17);
}

void StreamDeck::setBrightness(float brightness)
{
	const uint8_t brightnessData[17]{ 0x05, 0x55, 0xAA, 0xD1, 0x01, (uint8_t)(brightness * 111), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	sendFeatureReport(brightnessData, 17);
}

void StreamDeck::setColor(int buttonID, Colour color)
{
	
}

void StreamDeck::setImage(int buttonID, Image image)
{
	
}

void StreamDeck::sendButtonImageData(int buttonID, MemoryBlock data)
{
}

void StreamDeck::sendFeatureReport(const uint8_t* data, int length)
{
	if (device == nullptr)
	{
		NLOGWARNING("Stream Deck", "Trying to communicate with disconnected device");
		return;
	}

	try
	{
		hid_send_feature_report(device, data, length);
	}
	catch (std::exception e)
	{
		NLOGERROR("Stream Deck", "Error trying to communicate with device : " << e.what());
	}
}

void StreamDeck::run()
{
	unsigned char data[1024];
	while (!threadShouldExit() && device != nullptr)
	{
		try
		{
			int numRead = hid_read(device, data, 1024);
			if (numRead > 0)
			{
				DBG("Got data !");
				if (data[0] == 1)
				{
					DBG("Data is button states");
					for (int i = 1; i <= numButtons; i++) {
						bool state = data[i] > 0;
						int buttonID = i - 1;
						if (buttonStates[buttonID] != state)
						{
							buttonStates.set(buttonID, state);
							deviceListeners.call(state ? &StreamDeckListener::streamDeckButtonPressed : &StreamDeckListener::streamDeckButtonReleased, buttonID);
						}
					}
				}
			}
		}
		catch (std::exception e)
		{
			NLOGERROR("Stream Deck", "Error trying to read from device");
			device = nullptr;
			return;
		}
		
		sleep(20);
	}
}
