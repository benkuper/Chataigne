/*
  ==============================================================================

    MTCReceiver.h
    Created: 18 Nov 2019 9:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include <atomic>

class MTCReceiver :
	public MIDIInputDevice::MIDIInputListener,
	public MIDIManager::Listener,
	public Timer
{
public:
	MTCReceiver(MIDIInputDevice* device = nullptr);
	~MTCReceiver();

	std::atomic<bool> isPlaying;

	int hours;
	int minutes;
	int seconds;
	int frames;
	MidiMessage::SmpteTimecodeType type;
	double divider;

	int pieces[8]{};
	int piecesReceived = 0;
	std::atomic<double> lastQuarterFrameTime{ 0.0 };
	CriticalSection timeLock;
	double decodedTime = 0.0;
	

	void setDevice(MIDIInputDevice* newDevice);

	double getTime();

	void fullFrameTimecodeReceived(const MidiMessage &m) override;
	void quarterFrameTimecodeReceived(const MidiMessage &m) override;

	void midiDeviceInRemoved(MIDIInputDevice* d) override;

	void timerCallback() override;
	
	class MTCListener
	{
	public:
        virtual ~MTCListener() {}
		virtual void mtcTimeUpdated(bool /*fromFullFrame*/) {}
		virtual void mtcStarted() {}
		virtual void mtcStopped() {}
	};

	ListenerList<MTCListener> mtcListeners;
	void addMTCListener(MTCListener* newListener) { mtcListeners.add(newListener); }
	void removeMTCListener(MTCListener* listener) { mtcListeners.remove(listener); }

	

	MIDIInputDevice* device;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MTCReceiver)

};
