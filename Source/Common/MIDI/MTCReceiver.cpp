/*
  ==============================================================================

    MTCReceiver.cpp
    Created: 18 Nov 2019 9:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "MTCReceiver.h"

MTCReceiver::MTCReceiver(MIDIInputDevice* device) :
	isPlaying(false),
	hours(0), minutes(0), seconds(0), frames(0), type(MidiMessage::SmpteTimecodeType::fps30),
	device(nullptr)
{
	MIDIManager::getInstance()->addMIDIManagerListener(this);
	setDevice(device);
}

MTCReceiver::~MTCReceiver()
{
	if(MIDIManager::getInstanceWithoutCreating() != nullptr) MIDIManager::getInstance()->removeMIDIManagerListener(this);
	setDevice(nullptr);
}

void MTCReceiver::setDevice(MIDIInputDevice* newDevice)
{
	if (device == newDevice) return;

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
	return (double)(hours * 3600 + minutes * 60 + seconds) + (frames * 1.0 / divider);
}

void MTCReceiver::fullFrameTimecodeReceived(const MidiMessage& m)
{
	m.getFullFrameParameters(hours, minutes, seconds, frames, type);
	switch (type)
	{
	case MidiMessage::fps24: divider = 24;
	case MidiMessage::fps25: divider = 25;
	case MidiMessage::fps30: divider = 30;
	case MidiMessage::fps30drop: divider = 29.997;
	}

	
	mtcListeners.call(&MTCListener::mtcTimeUpdated, true);

}

void MTCReceiver::quarterFrameTimecodeReceived(const MidiMessage& m)
{
	int piece = m.getQuarterFrameSequenceNumber();
	pieces[piece] = m.getQuarterFrameValue();

	if ((Piece)piece == Piece::RateAndHourMSB)
	{
		frames = (pieces[(int)Piece::FrameLSB] & 0x0F) | ((pieces[(int)Piece::FrameMSB] & 0x01) << 4);
		seconds = (pieces[(int)Piece::SecondLSB] & 0x0F) | ((pieces[(int)Piece::SecondMSB] & 0x03) << 4);
		minutes = (pieces[(int)Piece::MinuteLSB] & 0x0F) | ((pieces[(int)Piece::MinuteMSB] & 0x03) << 4);
		hours = (pieces[(int)Piece::HourLSB] & 0x0F) | ((pieces[(int)Piece::RateAndHourMSB] & 0x01) << 4);
		MidiMessage::SmpteTimecodeType newType = (MidiMessage::SmpteTimecodeType)(pieces[(int)Piece::RateAndHourMSB] & 0x03);

		if (type != newType)
		{
			type = newType;
			switch (type)
			{
			case MidiMessage::fps24: divider = 24;
			case MidiMessage::fps25: divider = 25;
			case MidiMessage::fps30: divider = 30;
			case MidiMessage::fps30drop: divider = 29.997;
			}
		}

		if (!isPlaying)
		{
			isPlaying = true;
			mtcListeners.call(&MTCListener::mtcStarted);
		}

		startTimer((1000/divider)*2);
	}

	mtcListeners.call(&MTCListener::mtcTimeUpdated, false);
}

void MTCReceiver::midiDeviceInRemoved(MIDIInputDevice* d)
{
	if (d == device) setDevice(nullptr);
}

void MTCReceiver::timerCallback()
{
	isPlaying = false;
	mtcListeners.call(&MTCListener::mtcStopped);
	stopTimer();
}
