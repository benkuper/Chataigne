/*
  ==============================================================================

    StreamDeckMini.h
    Created: 14 Feb 2020 12:29:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StreamDeckMini :
	public StreamDeck
{
public:
	StreamDeckMini(hid_device* device, String serialNumbe);
	~StreamDeckMini();

	virtual void sendButtonImageData(int row, int column, Image& img) override;
};