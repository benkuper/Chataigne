/*
  ==============================================================================

    AjazzModule.h
    Created: 7 Apr 2026
    Author:  Andrea Bergamasco [@vjandrea]

  ==============================================================================
*/

#pragma once

#include "AjazzManager.h"
#include "AjazzDevice.h"

class AjazzModule :
	public Module,
	public AjazzManager::AjazzManagerListener,
	public AjazzDevice::AjazzListener
{
public:
	AjazzModule(const String& name = "Ajazz AKP");
	~AjazzModule();

	BoolParameter* isConnected;
	AjazzDevice* device;

	enum DeviceType { AKP153, AKP03, AKP05 };
	EnumParameter* deviceType;
	EnumParameter* targetDevice;
	StringParameter* deviceWarning;

	int numRows;
	int numColumns;
    int numSideKeys;
    int numExtraButtons;

	SpinLock rebuildingLock;

	Trigger* reset;
	BoolParameter* colorizeImages;
	BoolParameter* highlightPressedButtons;

	IntParameter* brightness;
	IntParameter* textSize;
	FloatParameter* knobSensitivity;
	FloatParameter* knobFineMultiplier; // sensitivity multiplier applied while a knob is held down
	FloatParameter* knobRangeMin;
	FloatParameter* knobRangeMax;
	BoolParameter* knobWrapAround; // wrap Absolute within [min,max] instead of clamping

	// Per-knob behavior config (only populated when knobsSupported)
	ControllableContainer knobSettingsCC;
	Array<BoolParameter*> knobInvert;

	ControllableContainer colorsCC;
	OwnedArray<Array<ColorParameter*>> colors;
	ControllableContainer imagesCC;
	OwnedArray<Array<FileParameter*>> images;
	ControllableContainer textsCC;
	OwnedArray<Array<StringParameter*>> texts;

	Array<ControllableContainer*> buttonRowsCC;
	OwnedArray<Array<BoolParameter*>> buttonStates;
    
    // Color/Image/Text for the side elements' own displays. Only meaningful when
    // sideIconsSupported is true: on AKP03 the knobs have no screen at all (confirmed by
    // hardware), so this is hidden and never sent there, even though the underlying
    // numSideKeys mechanism is shared with AKP153's real side displays.
    ControllableContainer sideCC;
    Array<ColorParameter*> sideColors;
    Array<FileParameter*> sideImages;
    Array<StringParameter*> sideTexts;
    bool sideIconsSupported;

    // Input values for the side buttons / knobs. "Pressed" applies to all models;
    // Absolute/Increment/Decrement are only built when knobsSupported is true (currently
    // AKP03 only, since AKP153 side elements are plain buttons and AKP05's protocol is
    // unverified). A rotary encoder has no true "home" position, so each detent fires an
    // Increment/Decrement trigger (Chataigne's native one-shot primitive) rather than a
    // Relative value that would otherwise linger at +1/-1 between ticks; Absolute is kept
    // as the one running, bounded number to bind against.
    ControllableContainer knobsCC;
    Array<BoolParameter*> sidePressed;
    Array<FloatParameter*> knobAbsolutes;
    Array<Trigger*> knobIncrementTriggers;
    Array<Trigger*> knobDecrementTriggers;
    bool knobsSupported;

    // A row of plain (non-rotary, no screen) buttons below the main grid on AKP03, separate
    // from both the grid keys and the knobs.
    ControllableContainer extraButtonsCC;
    Array<BoolParameter*> extraButtonsPressed;

	void rebuildValues();
	void updateDeviceList();
	void syncGeometry(DeviceType type, bool rebuildUI);
	void syncGeometryFromDevice(AjazzDevice* newDevice, bool rebuildUI);

	bool readyToSend;

	void setDevice(AjazzDevice* newDevice);
	void updateButton(int row, int column);
    void updateSideButton(int index);

	void setColor(int row, int column, const Colour& c);
	void setAllColor(const Colour& c);
	void setImage(int row, int column, const String& path);
	void setText(int row, int column, const String& text);
	void clearTexts();
    
    void setSideColor(int index, const Colour& c);
    void setSideImage(int index, const String& path);
    void setSideText(int index, const String& text);

	virtual void ajazzButtonPressed(int row, int column) override;
	virtual void ajazzButtonReleased(int row, int column) override;
    virtual void ajazzSideButtonPressed(int index) override;
    virtual void ajazzSideButtonReleased(int index) override;
    virtual void ajazzKnobRotated(int index, int direction) override;
    virtual void ajazzExtraButtonPressed(int index) override;
    virtual void ajazzExtraButtonReleased(int index) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	
	void deviceAdded(AjazzDevice* d) override;
	void deviceRemoved(AjazzDevice * d) override;

	void afterLoadJSONDataInternal() override;

	static AjazzModule * create() { return new AjazzModule(); }
	virtual String getDefaultTypeString() const override { return "Ajazz AKP"; }

	JUCE_DECLARE_WEAK_REFERENCEABLE(AjazzModule)
};
