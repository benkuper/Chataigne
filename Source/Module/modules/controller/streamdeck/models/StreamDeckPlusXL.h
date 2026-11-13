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

	// Both the keys and the strip show images turned 90 degrees clockwise
	static Image rotateCounterClockwise(const Image& img);
};
