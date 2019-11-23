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

class StreamDeckModule :
	public Module,
	public StreamDeckManager::AsyncListener,
	public StreamDeck::StreamDeckListener

{
public:
	StreamDeckModule(const String& name = "StreamDeck");
	~StreamDeckModule();

	BoolParameter * isConnected;
	StreamDeck* device;

	FloatParameter* brightness;
	ControllableContainer colorsCC;
	Array<ColorParameter*> colors;
	ControllableContainer imagesCC;
	Array<FileParameter*> images;

	Array<BoolParameter*> buttonStates;

	void rebuildValues();

	void setDevice(StreamDeck* newDevice);


	// Inherited via StreamDeckListener
	virtual void streamDeckButtonPressed(int button) override;
	virtual void streamDeckButtonReleased(int button) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	
	void newMessage(const StreamDeckManager::StreamDeckManagerEvent& e) override;

	static StreamDeckModule * create() { return new StreamDeckModule(); }
	virtual String getDefaultTypeString() const override { return "Stream Deck"; }
};

