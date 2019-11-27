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
	serialNumber(serialNumber)
{
	if(device != nullptr) hid_set_nonblocking(device, 1);
	for (int i = 0; i < STREAMDECK_MAX_BUTTONS; i++) buttonStates.add(false);

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
	sendFeatureReport(resetData, 17);
}

void StreamDeck::setBrightness(float brightness)
{
	brightnessData[5] = (uint8_t)(brightness * 100);
	sendFeatureReport(brightnessData, 17);
}

void StreamDeck::setColor(int buttonID, Colour color, bool highlight)
{
	if(highlight) color = color.brighter(1);
	uint8_t r = color.getRed();
	uint8_t g = color.getGreen();
	uint8_t b = color.getBlue();
	uint8_t data[ICON_BYTES];

	for (int i = 0; i < ICON_PIXELS; i++)
	{

		data[i * 3] = b;
		data[i * 3 + 1] = g;
		data[i * 3 + 2] = r;
	}


	sendButtonImageData(buttonID, data);
}

void StreamDeck::setImage(int buttonID, Image image, bool highlight)
{
	image = image.rescaled(ICON_SIZE, ICON_SIZE);
	Image iconImage(Image::RGB, ICON_SIZE, ICON_SIZE, true);

#if JUCE_MAC
	uint8_t data[ICON_BYTES];
#else
	image = image.convertedToFormat(Image::RGB);
#endif

	for (int tx = 0; tx < image.getWidth(); tx++)
	{
		for (int ty = 0; ty < image.getHeight(); ty++)
		{
			Colour ic = image.getPixelAt(image.getWidth() - tx, ty).brighter(highlight ? 1 : 0);
#if JUCE_MAC
			int index = (ty * image.getWidth() + tx) * 3;
			data[index] = ic.getRed();
			data[index+1] = ic.getGreen();
			data[index+2] = ic.getBlue();
#else
			iconImage.setPixelAt(tx, ty, ic);
#endif
		}
	}

#if JUCE_MAC
	sendButtonImageData(buttonID, data);

#else
	Image::BitmapData data(iconImage, Image::BitmapData::ReadWriteMode::readOnly);
	sendButtonImageData(buttonID, data.data);
#endif

}

void StreamDeck::setImage(int buttonID, Image image, Colour tint, bool highlight)
{
	image = image.rescaled(ICON_SIZE, ICON_SIZE).convertedToFormat(Image::RGB);
	Image iconImage(Image::RGB, ICON_SIZE, ICON_SIZE, true);

#if JUCE_MAC
	uint8_t data[ICON_BYTES];
#endif

	for (int tx = 0; tx < image.getWidth(); tx++)
	{
		for (int ty = 0; ty < image.getHeight(); ty++)
		{
			Colour ic = image.getPixelAt(image.getWidth()-tx, ty);
			ic = ic.withHue(tint.getHue()).withMultipliedBrightness(tint.getBrightness()).brighter(highlight?1:0);
#if JUCE_MAC
			int index = (ty = image.getWidth() + tx) * 3;
			data[index] = ic.getRed();
			data[index + 1] = ic.getGreen();
			data[index + 2] = ic.getBlue();
#else
			iconImage.setPixelAt(tx, ty, ic);
#endif
		}
	}

#if JUCE_MAC
	sendButtonImageData(buttonID, data);
#else
	Image::BitmapData data(iconImage, Image::BitmapData::ReadWriteMode::readOnly);
	sendButtonImageData(buttonID, data.data);
#endif
}


void StreamDeck::sendButtonImageData(int buttonID, const uint8_t* data)
{
	if(Engine::mainEngine->isClearing) return;
	writeLock.enter();

	page1Header[5] = buttonID+1;
	page2Header[5] = buttonID+1;

	MemoryBlock packet1;
	packet1.ensureSize(PACKET_SIZE, true);
	packet1.copyFrom(page1Header, 0, PACKET1_HEADER_SIZE);
	packet1.copyFrom(data, PACKET1_HEADER_SIZE, PACKET1_PIXELS_BYTES);

	MemoryBlock packet2;
	packet2.ensureSize(PACKET_SIZE, true);
	packet2.copyFrom(page2Header, 0, PACKET2_HEADER_SIZE);
	packet2.copyFrom(data + PACKET1_PIXELS_BYTES, PACKET2_HEADER_SIZE, PACKET2_PIXELS_BYTES);


	if (device != nullptr)
	{
		try {
			hid_write(device, (unsigned char*)packet1.getData(), PACKET_SIZE);
			hid_write(device, (unsigned char*)packet2.getData(), PACKET_SIZE);
		}
		catch (std::exception e)
		{
			NLOGERROR("StreamDeck", "Error write image to device");
		}
	}

	writeLock.exit();
}

void StreamDeck::sendFeatureReport(const uint8_t* data, int length)
{
	if (device == nullptr)
	{
		NLOGWARNING("Stream Deck", "Trying to communicate with disconnected device");
		return;
	}

	if (device != nullptr)
	{
		try
		{
			hid_send_feature_report(device, data, length);
		}
		catch (std::exception e)
		{
			NLOGERROR("Stream Deck", "Error trying to communicate with device : " << e.what());
		}
	}
}

void StreamDeck::run()
{
	unsigned char data[1024];
	while (!threadShouldExit() && device != nullptr)
	{
		
		try
		{
			int numRead = device != nullptr ? hid_read(device, data, 1024) : 0;
			if (numRead > 0)
			{
				if (data[0] == 1)
				{
					for (int i = 1; i <= STREAMDECK_MAX_BUTTONS; i++) {
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
