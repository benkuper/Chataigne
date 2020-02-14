/*
  ==============================================================================

    StreamDeckMini.cpp
    Created: 14 Feb 2020 12:29:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckMini.h"

StreamDeckMini::StreamDeckMini(hid_device* device, String serialNumber) :
	StreamDeck(device, serialNumber, MINI, 3, 2, false, 72, 1)
{
}

StreamDeckMini::~StreamDeckMini()
{
}

void StreamDeckMini::writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength)
{
}