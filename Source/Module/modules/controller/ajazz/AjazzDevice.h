/*
  ==============================================================================

    AjazzDevice.h
    Created: 7 Apr 2026 
    Author:  Andrea Bergamasco [@vjandrea]

  ==============================================================================
*/

#pragma once

#include "hidapi.h"

class AjazzDevice :
	public Thread
{
public:
	AjazzDevice(hid_device* device, String serialNumber, String devicePath, int pid);
	virtual ~AjazzDevice();

	hid_device* device;
	String serialNumber;
	String devicePath;
	int pid;
	String modelName;

	//Data
	int numKeys;
	int numRows;
	int numColumns;
    int numSideKeys;
    int numExtraButtons;

	Array<bool> buttonStates;
	SpinLock writeLock;

	void reset();
	void setBrightness(int percent);
	void clearButton(int buttonId);
    void clearAll();

	void setImage(int row, int column, Image image, bool highlight, const String& overlayText = "", int textSize = 10, bool colorize = false, Colour bgColor = Colours::black);
	void setSideImage(int index, Image image, bool highlight, const String& overlayText = "", int textSize = 10, bool colorize = false, Colour bgColor = Colours::black);

	void sendButtonImageData(int buttonId, Image& img);

	void run() override;

	class AjazzListener
	{
	public:
		virtual ~AjazzListener() {}
		virtual void ajazzButtonPressed(int row, int column) = 0;
		virtual void ajazzButtonReleased(int row, int column) = 0;
        virtual void ajazzSideButtonPressed(int index) {}
        virtual void ajazzSideButtonReleased(int index) {}
        virtual void ajazzKnobRotated(int index, int direction) {} // direction: 1 = CW, -1 = CCW
        virtual void ajazzExtraButtonPressed(int index) {}
        virtual void ajazzExtraButtonReleased(int index) {}
	};

	ListenerList<AjazzListener> deviceListeners;
	void addAjazzListener(AjazzListener* newListener) { deviceListeners.add(newListener); }
	void removeAjazzListener(AjazzListener* listener) { deviceListeners.remove(listener); }

private:
    bool deviceInitialized;
    int currentBrightness;
    void sendInitPacketIfNeeded();
    void sendPacket(const uint8_t* data, int length);
    int getButtonProtocolId(int row, int column);
    void getImageFormat(int& imgSize, int& rotationMode) const;

    JUCE_DECLARE_WEAK_REFERENCEABLE(AjazzDevice)
};
