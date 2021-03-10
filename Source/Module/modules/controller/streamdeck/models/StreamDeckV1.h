/*
  ==============================================================================

    StreamDeckV1.h
    Created: 14 Feb 2020 12:29:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StreamDeckV1 :
	public StreamDeck
{
public:
	StreamDeckV1(hid_device* device, String serialNumber);
	~StreamDeckV1();

	// Inherited via StreamDeck
	virtual void sendButtonImageData(int row, int column, Image& img) override;
};

