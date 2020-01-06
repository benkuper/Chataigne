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
	StreamDeck(hid_device * device, String serialNumber, int pid);
	virtual ~StreamDeck();

	enum Model { MINI, STANDARD_V1, STANDARD_V2, XL};
	Model model;

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

	int imagePacketLength;
	int imagePacketHeaderLength; 
	int iconSize;
	int keyDataOffset;
	bool invertX;
	
	Array<uint8_t> resetData;
	Array<uint8_t> brightnessData;
	Array<uint8_t> page1Header;
	Array<uint8_t> page2Header;
};