/*
  ==============================================================================

    MTCReceiver.h
    Created: 18 Nov 2019 9:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "MIDIDevice.h"
#include "MIDIManager.h"

class MTCReceiver :
	public MIDIInputDevice::MIDIInputListener,
	public MIDIManager::Listener,
	public Timer
{
public:
	MTCReceiver(MIDIInputDevice* device = nullptr);
	~MTCReceiver();

	bool isPlaying;

	int hours;
	int minutes;
	int seconds;
	int frames;
	MidiMessage::SmpteTimecodeType type;
	double divider;

	enum class Piece {
		FrameLSB = 0,
		FrameMSB,
		SecondLSB,
		SecondMSB,
		MinuteLSB,
		MinuteMSB,
		HourLSB,
		RateAndHourMSB
	};

	int pieces[8];
	

	void setDevice(MIDIInputDevice* newDevice);

	double getTime();

	void fullFrameTimecodeReceived(const MidiMessage &m) override;
	void quarterFrameTimecodeReceived(const MidiMessage &m) override;

	void midiDeviceInRemoved(MIDIInputDevice* d) override;

	void timerCallback() override;
	
	class MTCListener
	{
	public:
		virtual void mtcTimeUpdated(bool fromFullFrame) {}
		virtual void mtcStarted() {}
		virtual void mtcStopped() {}
	};

	ListenerList<MTCListener> mtcListeners;
	void addMTCListener(MTCListener* newListener) { mtcListeners.add(newListener); }
	void removeMTCListener(MTCListener* listener) { mtcListeners.remove(listener); }

	

	MIDIInputDevice* device;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MTCReceiver)

};
