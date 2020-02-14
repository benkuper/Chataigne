/*
  ==============================================================================

    StreamDeckXL.h
    Created: 14 Feb 2020 12:29:21pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../StreamDeck.h"

class StreamDeckXL :
	public StreamDeck
{
public:
	StreamDeckXL(hid_device* device, String serialNumber);
	~StreamDeckXL();

	// Inherited via StreamDeck
	virtual void writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength) override;
};