/*
  ==============================================================================

	StreamDeckPedal.h
	Created: 16 Nov 2026 10:00:00am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class StreamDeckPedal :
	public StreamDeck
{
public:
	StreamDeckPedal(hid_device* device, String serialNumber);
	~StreamDeckPedal();

	virtual void setColor(int row, int column, Colour color, bool highlight, const String& overlayText = "", int textSize = 10) override;
	virtual void setImage(int row, int column, Image image, bool highlight, const String& overlayText = "", int textSize = 10) override;
	virtual void setImage(int row, int column, Image image, Colour tint, bool highlight, const String& overlayText = "", int textSize = 10) override;
	virtual void sendButtonImageData(int row, int column, Image& img) override;

	virtual void sendFeatureReport(const uint8_t* data, int length) override;
};
