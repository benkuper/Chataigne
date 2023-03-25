/*
 ==============================================================================

 Copyright © Organic Orchestra, 2017

 This file is part of LGML. LGML is a software to manipulate sound in real-time

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation (version 3 of the License).

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 ==============================================================================
 */

#include "Module/ModuleIncludes.h"

MIDIClockSender::MIDIClockSender() :
	Thread("Clock Sender"),
	device(nullptr),
	bpm(0),
	waitInterval(0)
{
}

MIDIClockSender::~MIDIClockSender()
{
}

void MIDIClockSender::setBPM(double newBPM)
{
	bpm = newBPM;
	const int ticksPerBeat = 24;
	waitInterval = 60000 / (bpm * ticksPerBeat);
}

void MIDIClockSender::setOutDevice(MidiOutput* outDevice)
{
	bool isRunning = isThreadRunning();
	stop();
	device = outDevice;

	if (isRunning) start();
}

void MIDIClockSender::start()
{
	startThread();
}

void MIDIClockSender::stop()
{
	stopThread(1000);
}

void MIDIClockSender::run()
{
	if (device == nullptr) return;

	device->sendMessageNow(MidiMessage::midiStart());
	while (!threadShouldExit())
	{
		if (bpm == 0)
		{
			wait(10);
			continue;
		}

		device->sendMessageNow(MidiMessage::midiClock());
		wait(waitInterval);
	}

	if (Engine::mainEngine->isClearing || device == nullptr)  return;
	
	device->sendMessageNow(MidiMessage::midiStop());
}
