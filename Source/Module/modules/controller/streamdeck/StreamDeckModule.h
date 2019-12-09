/*
  ==============================================================================

    StreamDeckModule.h
    Created: 22 Nov 2019 4:37:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "StreamDeckManager.h"

#define USE_FAKE_DEVICE 0

class StreamDeckModule :
	public Module,
	public StreamDeckManager::StreamDeckManagerListener,
	public StreamDeck::StreamDeckListener

{
public:
	StreamDeckModule(const String& name = "StreamDeck");
	~StreamDeckModule();

	BoolParameter* isConnected;
	StreamDeck* device;

	enum DeviceType { STANDARD = 15, MINI = 6, XL = 32 };
	EnumParameter* deviceType;

	Trigger* reset;
	BoolParameter* colorizeImages;
	BoolParameter* highlightPressedButtons;

	FloatParameter* brightness;
	ControllableContainer colorsCC;
	Array<ColorParameter*> colors;
	ControllableContainer imagesCC;
	Array<FileParameter*> images;

	Array<BoolParameter*> buttonStates;

#if USE_FAKE_DEVICE
	StreamDeck fakeDevice;
#endif

	void rebuildValues();

	void setDevice(StreamDeck* newDevice);
	void updateButton(int id);

	virtual void streamDeckButtonPressed(int button) override;
	virtual void streamDeckButtonReleased(int button) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	
	void deviceAdded(StreamDeck* d) override;
	void deviceRemoved(StreamDeck * d) override;

	static StreamDeckModule * create() { return new StreamDeckModule(); }
	virtual String getDefaultTypeString() const override { return "Stream Deck"; }
};