/*
  ==============================================================================

    StreamDeckModule.h
    Created: 22 Nov 2019 4:37:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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

	int numRows;
	int numColumns;

	SpinLock rebuildingLock;

	Trigger* reset;
	BoolParameter* colorizeImages;
	BoolParameter* highlightPressedButtons;

	FloatParameter* brightness;
	IntParameter* textSize;

	ControllableContainer colorsCC;
	OwnedArray<Array<ColorParameter*>> colors;
	ControllableContainer imagesCC;
	OwnedArray<Array<FileParameter*>> images;
	ControllableContainer textsCC;
	OwnedArray<Array<StringParameter*>> texts;

	Array<ControllableContainer*> buttonRowsCC;
	OwnedArray<Array<BoolParameter*>> buttonStates;

#if USE_FAKE_DEVICE
	StreamDeck fakeDevice;
#endif

	void rebuildValues();

	void setDevice(StreamDeck* newDevice);
	void updateButton(int row, int column);

	void setColor(int row, int column, const Colour& c);
	void setAllColor(const Colour& c);
	void setImage(int row, int column, const String& path);
	void setText(int row, int column, const String& text);
	void clearTexts();

	virtual void streamDeckButtonPressed(int row, int column) override;
	virtual void streamDeckButtonReleased(int row, int column) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	
	void deviceAdded(StreamDeck* d) override;
	void deviceRemoved(StreamDeck * d) override;

	void afterLoadJSONDataInternal() override;

	static StreamDeckModule * create() { return new StreamDeckModule(); }
	virtual String getDefaultTypeString() const override { return "Stream Deck"; }
};