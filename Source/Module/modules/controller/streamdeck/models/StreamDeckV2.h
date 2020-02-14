/*
  ==============================================================================

    StreamDeckV2.h
    Created: 14 Feb 2020 12:29:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../StreamDeck.h"

class StreamDeckV2 :
	public StreamDeck
{
public:
	StreamDeckV2(hid_device* device, String serialNumber);
	~StreamDeckV2();

	void setBrightnessInternal(float brightness) override;

	// Inherited via StreamDeck
	virtual void writeImageDataHeader(MemoryOutputStream &stream, int keyIndex, int partIndex, bool isLast, int bodyLength) override;

	virtual void writeImageData(MemoryOutputStream &stream, Image& img) override;
};