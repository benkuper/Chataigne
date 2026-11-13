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
	public StreamDeck::StreamDeckListener,
	public Timer
{
public:
	StreamDeckModule(const String& name = "StreamDeck");
	~StreamDeckModule();

	BoolParameter* isConnected;
	StreamDeck* device;

	enum DeviceType { STANDARD = 15, MINI = 6, XL = 32, PLUS_XL = 36 };
	EnumParameter* deviceType;

	int numRows;
	int numColumns;
	int numDials;

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

	// Dials and touch strip (Stream Deck + XL)
	ControllableContainer dialSettingsCC;
	FloatParameter* dialSensitivity;
	FloatParameter* dialFineMultiplier;
	FloatParameter* dialRangeMin;
	FloatParameter* dialRangeMax;
	BoolParameter* dialWrapAround;
	Array<StringParameter*> dialLabels;
	Array<ColorParameter*> dialColors;

	ControllableContainer dialsCC;
	Array<BoolParameter*> dialPressed;
	Array<FloatParameter*> dialValues;
	Array<Trigger*> dialIncrements;
	Array<Trigger*> dialDecrements;

	ControllableContainer stripCC;
	FloatParameter* stripPosition;
	Trigger* stripTap;
	Trigger* stripLongPress;
	Trigger* stripSwipeLeft;
	Trigger* stripSwipeRight;

	std::atomic<bool> stripNeedsRedraw;

#if USE_FAKE_DEVICE
	StreamDeck fakeDevice;
#endif

	void rebuildValues();
	void rebuildDials();
	void timerCallback() override;

	void setDevice(StreamDeck* newDevice);
	void updateButton(int row, int column);

	void setColor(int row, int column, const Colour& c);
	void setAllColor(const Colour& c);
	void setImage(int row, int column, const String& path);
	void setText(int row, int column, const String& text);
	void clearTexts();

	virtual void streamDeckButtonPressed(int row, int column) override;
	virtual void streamDeckButtonReleased(int row, int column) override;
	virtual void streamDeckEncoderRotated(int index, int delta) override;
	virtual void streamDeckEncoderPressed(int index) override;
	virtual void streamDeckEncoderReleased(int index) override;
	virtual void streamDeckStripTapped(float x, bool longPress) override;
	virtual void streamDeckStripSwiped(bool toRight) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	
	void deviceAdded(StreamDeck* d) override;
	void deviceRemoved(StreamDeck * d) override;

	void afterLoadJSONDataInternal() override;

	static StreamDeckModule * create() { return new StreamDeckModule(); }
	virtual String getDefaultTypeString() const override { return "Stream Deck"; }
};