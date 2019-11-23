/*
  ==============================================================================

    StreamDeckManager.h
    Created: 23 Nov 2019 2:48:42am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "StreamDeck.h"

class StreamDeckManager :
	public Timer
{
public:
	juce_DeclareSingleton(StreamDeckManager, true)

	StreamDeckManager();
	~StreamDeckManager();

	const int vid = 4057;
	const int pid = 96;

	OwnedArray<StreamDeck> devices;

	void checkDevices();
	StreamDeck* getItemWithSerial(StringRef serial);
	StreamDeck* getItemWithHidDevice(hid_device* device);

	StreamDeck* openDevice(hid_device_info* deviceInfo);

	virtual void timerCallback() override;

	class StreamDeckManagerEvent
	{
	public:
		enum Type { DEVICES_CHANGED };
		StreamDeckManagerEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<StreamDeckManagerEvent> queuedNotifier;
	typedef QueuedNotifier<StreamDeckManagerEvent>::Listener AsyncListener;
	void addAsyncManagerListener(AsyncListener* newListener) { queuedNotifier.addListener(newListener); }
	void addAsyncCoalescedManagerListener(AsyncListener* newListener) { queuedNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncManagerListener(AsyncListener* listener) { queuedNotifier.removeListener(listener); }
};