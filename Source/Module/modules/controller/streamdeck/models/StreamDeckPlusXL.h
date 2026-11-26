/*
  ==============================================================================

    StreamDeckPlusXL.h
    Stream Deck + XL : 36 keys (4x9), 6 push dials, 1200x100 touch strip

  ==============================================================================
*/

#pragma once

class StreamDeckPlusXL :
	public StreamDeck
{
public:
	StreamDeckPlusXL(hid_device* device, String serialNumber);
	~StreamDeckPlusXL() {}

	static const int stripWidth = 1200;
	static const int stripHeight = 100;

	Array<bool> encoderStates;

	void setBrightnessInternal(float brightness) override;

	void writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength) override;
	void writeImageData(MemoryOutputStream& stream, Image& img) override;

	void setEncoderDisplays(const Array<EncoderDisplay>& displays) override;
	void sendStripImage(const Image& img);

	void processInputReport(const unsigned char* data, int length) override;

	// Per Elgato's HID reference (docs.elgato.com/streamdeck/hid/stream-deck-plus-xl),
	// both the keys and the strip take images rotated 90 degrees counterclockwise
	static Image rotateCounterClockwise(const Image& img);
};
