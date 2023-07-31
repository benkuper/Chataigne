/*
  ==============================================================================

	StreamDeck.cpp
	Created: 23 Nov 2019 2:48:28am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

StreamDeck::StreamDeck(hid_device* device, String serialNumber, Model model, int numColumns, int numRows, bool invertX, int iconSize, int keyDataOffset) :
	Thread("StreamDeck"),
	model(model),
	device(device),
	serialNumber(serialNumber),
	numKeys(numRows* numColumns),
	numRows(numRows),
	numColumns(numColumns),
	invertX(keyDataOffset),
	iconSize(iconSize),
	keyDataOffset(keyDataOffset),
	imagePacketLength(0),
	imageHeaderLength(0)
{

	if (device != nullptr) hid_set_nonblocking(device, 1);
	for (int i = 0; i < numKeys; ++i) buttonStates.add(false);

	startThread();
}

StreamDeck::~StreamDeck()
{
	stopThread(500);
}

void StreamDeck::reset()
{
	sendFeatureReport(resetData.getRawDataPointer(), resetData.size());
}

void StreamDeck::setBrightness(float brightness)
{
	setBrightnessInternal(brightness);
	sendFeatureReport(brightnessData.getRawDataPointer(), brightnessData.size());
}

void StreamDeck::setColor(int row, int column, Colour color, bool highlight, const String& overlayText, int textSize)
{
	if (highlight) color = color.brighter(1);
	//const int numPixels = iconSize * iconSize;
	Image iconImage(Image::RGB, iconSize, iconSize, true);
	Graphics g(iconImage);
	g.setColour(color);
	g.fillAll();

	if (overlayText.isNotEmpty())
	{
		g.setColour(color.getPerceivedBrightness() > .5f ? Colours::black : Colours::white);
		g.setFont(textSize);
		g.drawFittedText(overlayText, g.getClipBounds().reduced(2), Justification::centred, 5);
	}

	sendButtonImageData(row, column, iconImage);
}

void StreamDeck::setImage(int row, int column, Image image, bool highlight, const String& overlayText, int textSize)
{
	setImage(row, column, image, Colours::black, highlight, overlayText, textSize);
}

void StreamDeck::setImage(int row, int column, Image image, Colour tint, bool highlight, const String& overlayText, int textSize)
{
	Image iconImage(Image::RGB, iconSize, iconSize, true);
	Graphics g(iconImage);
	g.setColour(Colours::black);
	g.fillAll();
	g.drawImage(image, g.getClipBounds().toFloat());
	g.setColour(tint.withMultipliedAlpha(.5f).brighter((float)highlight));
	g.fillAll();

	if (overlayText.isNotEmpty())
	{
		g.setColour(tint.getPerceivedBrightness() > .5f ? Colours::black : Colours::white);
		g.setFont(textSize);
		g.drawFittedText(overlayText, g.getClipBounds().reduced(5), Justification::centred, 5);
	}

	sendButtonImageData(row, column, iconImage);
}

void StreamDeck::writeImageData(MemoryOutputStream& stream, Image& img)
{
	Image::BitmapData bitmapData(img, Image::BitmapData::ReadWriteMode::readOnly);
	stream.write(bitmapData.data, getIconBytes());
}


void StreamDeck::sendButtonImageData(int row, int column, Image& img)
{
	if (Engine::mainEngine->isClearing) return;

	writeLock.enter();
	MemoryOutputStream imageData;
	writeImageData(imageData, img);

	const int payload = imagePacketLength - imageHeaderLength;
	int remainingBytes = (int)imageData.getDataSize();

	int buttonID = row * numColumns + column;
	int byteOffset = 0;

	for (int part = 0; remainingBytes > 0; part++)
	{
		MemoryOutputStream partStream;

		int numPartBytes = jmin(remainingBytes, payload);

		writeImageDataHeader(partStream, buttonID, part, remainingBytes <= imagePacketLength, numPartBytes);

		partStream.write((uint8*)imageData.getData() + byteOffset, numPartBytes);
		partStream.writeRepeatedByte(0, imagePacketLength - partStream.getDataSize());

		hid_write(device, (unsigned char*)partStream.getData(), imagePacketLength);

		byteOffset += numPartBytes;
		remainingBytes -= numPartBytes;
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
					for (int i = 0; i < numKeys; ++i)
					{
						bool state = data[i + keyDataOffset] > 0;

						if (buttonStates[i] != state)
						{
							buttonStates.set(i, state);
							int column = i % numColumns;
							int row = floor(i * 1.0f / numColumns);
							deviceListeners.call(state ? &StreamDeckListener::streamDeckButtonPressed : &StreamDeckListener::streamDeckButtonReleased, row, column);
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

		wait(20);
	}
}
