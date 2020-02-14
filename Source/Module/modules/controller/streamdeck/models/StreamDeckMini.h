/*
  ==============================================================================

    StreamDeckMini.h
    Created: 14 Feb 2020 12:29:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../StreamDeck.h"

class StreamDeckMini :
	public StreamDeck
{
public:
	StreamDeckMini(hid_device* device, String serialNumbe);
	~StreamDeckMini();

	// Inherited via StreamDeck
	virtual void writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength) override;
};