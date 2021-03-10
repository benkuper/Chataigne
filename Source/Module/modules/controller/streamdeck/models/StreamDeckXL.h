/*
  ==============================================================================

    StreamDeckXL.h
    Created: 14 Feb 2020 12:29:21pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StreamDeckXL :
	public StreamDeck
{
public:
	StreamDeckXL(hid_device* device, String serialNumber);
	~StreamDeckXL();

	void setBrightnessInternal(float brightness) override;

	// Inherited via StreamDeck
	virtual void writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength) override;

	virtual void writeImageData(MemoryOutputStream& stream, Image& img) override;
};