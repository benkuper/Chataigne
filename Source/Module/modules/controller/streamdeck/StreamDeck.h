/*
  ==============================================================================

    StreamDeck.h
    Created: 23 Nov 2019 2:48:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "hidapi.h"

class StreamDeck :
	public Thread
{
public:
	StreamDeck(hid_device * device, String serialNumber);
	~StreamDeck();

	hid_device* device;
	String serialNumber;

	int numButtons;
	int imageSize;
	Array<bool> buttonStates;

	void reset();
	void setBrightness(float brightness);

	void setColor(int buttonID, Colour color);
	void setImage(int buttonID, Image image);

	void sendButtonImageData(int buttonID, MemoryBlock data);
	void sendFeatureReport(const uint8_t* data, int length);

	void run() override;

	class StreamDeckListener
	{
	public:
		virtual ~StreamDeckListener() {}
		virtual void streamDeckButtonPressed(int button) = 0;
		virtual void streamDeckButtonReleased(int button) = 0;

	};

	ListenerList<StreamDeckListener> deviceListeners;
	void addStreamDeckListener(StreamDeckListener* newListener) { deviceListeners.add(newListener); }
	void removeStreamDeckListener(StreamDeckListener* listener) { deviceListeners.remove(listener); }
};