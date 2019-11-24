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


#define ICON_SIZE 72
#define ICON_PIXELS ICON_SIZE*ICON_SIZE
#define ICON_BYTES ICON_PIXELS * 3

#define PACKET1_HEADER_SIZE 70
#define PACKET1_PIXELS 2583   
#define PACKET1_PIXELS_BYTES PACKET1_PIXELS * 3   

#define PACKET2_HEADER_SIZE 16
#define PACKET2_PIXELS 2601  
#define PACKET2_PIXELS_BYTES PACKET2_PIXELS * 3   

#define PACKET_SIZE 8191

#define STREAMDECK_MAX_BUTTONS 32

class StreamDeck :
	public Thread
{
public:
	StreamDeck(hid_device * device, String serialNumber);
	virtual ~StreamDeck();

	hid_device* device;
	String serialNumber;

	Array<bool> buttonStates;

	SpinLock writeLock;

	void reset();
	void setBrightness(float brightness);

	void setColor(int buttonID, Colour color, bool highlight);
	void setImage(int buttonID, Image image, bool highlight);
	void setImage(int buttonID, Image image, Colour tint, bool highlight);

	void sendButtonImageData(int buttonID, const uint8_t * data);
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


private:

	const uint8_t resetData[17]{ 0x0B, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t brightnessData[17]{ 0x05, 0x55, 0xAA, 0xD1, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	uint8_t page1Header[70]{
			0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x4D, 0xF6,
				0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00,
				0x48, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3C, 0x00, 0x00, 0x13, 0x0E, 0x00,
				0x00, 0x13, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	uint8_t page2Header[16]{
			0x02, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
};