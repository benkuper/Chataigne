/*
  ==============================================================================

    StreamDeckXL.cpp
    Created: 14 Feb 2020 12:29:21pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckXL.h"

StreamDeckXL::StreamDeckXL(hid_device* device, String serialNumber) :
	StreamDeck(device, serialNumber, XL, 8, 4, true, 96, 4)
{
}

StreamDeckXL::~StreamDeckXL()
{
}

void StreamDeckXL::writeImageDataHeader(MemoryOutputStream& stream, int keyIndex, int partIndex, bool isLast, int bodyLength)
{
}