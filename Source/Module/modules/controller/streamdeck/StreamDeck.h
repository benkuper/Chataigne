/*
  ==============================================================================

	StreamDeck.h
	Created: 23 Nov 2019 2:48:28am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "hidapi.h"

#define STREAMDECK_MAX_ROWS 4
#define STREAMDECK_MAX_COLUMNS 9

class StreamDeck :
	public Thread
{
public:
	enum Model { MINI, STANDARD_V1, STANDARD_V2, XL, PLUS_XL };

	// What to draw above one dial on models with a touch strip
	struct EncoderDisplay
	{
		String label;
		Colour colour;
		String valueText;
		float normalizedValue; // 0..1, drawn as a bar
	};

	StreamDeck(hid_device* device, String serialNumber, Model model, int numColumns, int numRows, bool invertX, int iconSize, int keyDataOffset);
	virtual ~StreamDeck();

	Model model;
	hid_device* device;
	String serialNumber;

	//Data
	int numKeys;
	int numRows;
	int numColumns;
	bool invertX;
	int iconSize;
	int keyDataOffset;
	int imagePacketLength;
	int imageHeaderLength;
	int numEncoders = 0;

	Array<uint8_t> resetData;
	Array<uint8_t> brightnessData;
	Array<uint8_t> page1Header;
	Array<uint8_t> page2Header;

	Array<bool> buttonStates;
	SpinLock writeLock;

	void reset();
	void setBrightness(float brightness);
	virtual void setBrightnessInternal(float brightness) {}

	virtual void setColor(int row, int column, Colour color, bool highlight, const String& overlayText = "", int textSize = 10);
	virtual void setImage(int row, int column, Image image, bool highlight, const String& overlayText = "", int textSize = 10);
	virtual void setImage(int row, int column, Image image, Colour tint, bool highlight, const String& overlayText = "", int textSize = 10);
	virtual void getFirmwareVersion() {}

	int getIconBytes() const { return iconSize * iconSize * 3; }

	virtual void sendButtonImageData(int row, int column, Image& img);
	virtual void writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength) {}
	virtual void writeImageData(MemoryOutputStream& stream, Image& img);

	virtual void sendFeatureReport(const uint8_t* data, int length);

	// Models with dials and a touch strip draw one box per dial on the strip
	virtual void setEncoderDisplays(const Array<EncoderDisplay>& displays) {}

	virtual void processInputReport(const unsigned char* data, int length);

	void run() override;

	class StreamDeckListener
	{
	public:
		virtual ~StreamDeckListener() {}
		virtual void streamDeckButtonPressed(int row, int column) = 0;
		virtual void streamDeckButtonReleased(int row, int column) = 0;

		virtual void streamDeckEncoderRotated(int index, int delta) {}
		virtual void streamDeckEncoderPressed(int index) {}
		virtual void streamDeckEncoderReleased(int index) {}

		// x is normalized 0..1 along the strip, left to right
		virtual void streamDeckStripTapped(float x, bool longPress) {}
		virtual void streamDeckStripSwiped(bool toRight) {}
	};

	ListenerList<StreamDeckListener> deviceListeners;
	void addStreamDeckListener(StreamDeckListener* newListener) { deviceListeners.add(newListener); }
	void removeStreamDeckListener(StreamDeckListener* listener) { deviceListeners.remove(listener); }


};