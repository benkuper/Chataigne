/*
  ==============================================================================

    MTCReceiver.cpp
    Created: 18 Nov 2019 9:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "TimecodeMath.h"

MTCReceiver::MTCReceiver(MIDIInputDevice* device) :
	isPlaying(false),
	hours(0), minutes(0), seconds(0), frames(0), type(MidiMessage::SmpteTimecodeType::fps30),
	divider(30.0),
	device(nullptr)
{
	MIDIManager::getInstance()->addMIDIManagerListener(this);
	setDevice(device);
}

MTCReceiver::~MTCReceiver()
{
	if(MIDIManager::getInstanceWithoutCreating() != nullptr) MIDIManager::getInstance()->removeMIDIManagerListener(this);
	stopTimer();
	setDevice(nullptr);
}

void MTCReceiver::setDevice(MIDIInputDevice* newDevice)
{
	if (device == newDevice) return;
	stopTimer();
	isPlaying = false;
	piecesReceived = 0;

	if (device != nullptr)
	{
		device->removeMIDIInputListener(this);
	}

	device = newDevice;

	if (device != nullptr)
	{
		device->addMIDIInputListener(this);
	}
}

double MTCReceiver::getTime()
{
	const ScopedLock scopedLock(timeLock);
	return decodedTime;
}

void MTCReceiver::fullFrameTimecodeReceived(const MidiMessage& m)
{
	int newHours, newMinutes, newSeconds, newFrames;
	MidiMessage::SmpteTimecodeType newType;
	m.getFullFrameParameters(newHours, newMinutes, newSeconds, newFrames, newType);
	if (newHours >= 24 || newMinutes >= 60 || newSeconds >= 60 || newFrames >= TimecodeMath::nominalFPS(static_cast<int>(newType))) return;
	{
		const ScopedLock scopedLock(timeLock);
		hours = newHours;
		minutes = newMinutes;
		seconds = newSeconds;
		frames = newFrames;
		type = newType;
		divider = TimecodeMath::actualFPS(static_cast<int>(type));
		decodedTime = TimecodeMath::toSeconds(hours, minutes, seconds, frames, static_cast<int>(type));
	}
	piecesReceived = 0;
	mtcListeners.call(&MTCListener::mtcTimeUpdated, true);
}

void MTCReceiver::quarterFrameTimecodeReceived(const MidiMessage& m)
{
	int piece = m.getQuarterFrameSequenceNumber();
	const double now = Time::getMillisecondCounterHiRes();
	if (now - lastQuarterFrameTime.load() > 500.0) piecesReceived = 0;
	lastQuarterFrameTime = now;

	if (piece < 0 || piece > 7) return;
	if (piece != piecesReceived)
	{
		piecesReceived = 0;
		if (piece != 0) return;
	}
	pieces[piece] = m.getQuarterFrameValue();
	if (++piecesReceived != 8) return;
	piecesReceived = 0;

	const int newFrames = (pieces[0] & 0x0f) | ((pieces[1] & 0x01) << 4);
	const int newSeconds = (pieces[2] & 0x0f) | ((pieces[3] & 0x03) << 4);
	const int newMinutes = (pieces[4] & 0x0f) | ((pieces[5] & 0x03) << 4);
	const int newHours = (pieces[6] & 0x0f) | ((pieces[7] & 0x01) << 4);
	const auto newType = static_cast<MidiMessage::SmpteTimecodeType>((pieces[7] >> 1) & 0x03);
	if (newHours >= 24 || newMinutes >= 60 || newSeconds >= 60 || newFrames >= TimecodeMath::nominalFPS(static_cast<int>(newType))) return;

	{
		const ScopedLock scopedLock(timeLock);
		hours = newHours;
		minutes = newMinutes;
		seconds = newSeconds;
		frames = newFrames;
		type = newType;
		divider = TimecodeMath::actualFPS(static_cast<int>(type));
		// An eight-piece MTC cycle describes the time at its start, two frames ago.
		decodedTime = std::fmod(TimecodeMath::toSeconds(hours, minutes, seconds, frames, static_cast<int>(type)) + 2.0 / divider, 86400.0);
	}

	mtcListeners.call(&MTCListener::mtcTimeUpdated, false);
	if (!isPlaying.exchange(true))
	{
		startTimer(100);
		mtcListeners.call(&MTCListener::mtcStarted);
	}
}

void MTCReceiver::midiDeviceInRemoved(MIDIInputDevice* d)
{
	if (d == device)
	{
		const bool wasPlaying = isPlaying;
		setDevice(nullptr);
		if (wasPlaying) mtcListeners.call(&MTCListener::mtcStopped);
	}
}

void MTCReceiver::timerCallback()
{
	if (Time::getMillisecondCounterHiRes() - lastQuarterFrameTime.load() < 500.0) return;
	if (isPlaying.exchange(false)) mtcListeners.call(&MTCListener::mtcStopped);
	stopTimer();
}
