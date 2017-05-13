/*
  ==============================================================================

    MIDIManager.cpp
    Created: 20 Dec 2016 12:33:33pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIManager.h"

juce_ImplementSingleton(MIDIManager)

MIDIManager::MIDIManager()
{
	startTimer(500); //check devices each half seconds
	checkDevices();
}

MIDIManager::~MIDIManager()
{
}

void MIDIManager::checkDevices()
{

	//INPUTS
	StringArray inputNames = MidiInput::getDevices();

	Array<MIDIInputDevice *> inputDevicesToRemove;
	for (auto &d : inputs)
	{
		if (!inputNames.contains(d->name))
		{
			inputDevicesToRemove.add(d);
		}
	}

	for(auto &d : inputDevicesToRemove)
	{
		removeInputDevice(d);
	}

	for (auto &n : inputNames)
	{
		addInputDeviceIfNotThere(n);
	}
	

	//OUTPUTS
	StringArray outputNames = MidiOutput::getDevices();

	Array<MIDIOutputDevice *> outputDevicesToRemove;
	for (auto &d : outputs)
	{
		if (!outputNames.contains(d->name))
		{
			outputDevicesToRemove.add(d);
		}
	}

	for (auto &d : outputDevicesToRemove)
	{
		removeOutputDevice(d);
	}


	for (auto &n : outputNames)
	{
		addOutputDeviceIfNotThere(n);
	}
}

void MIDIManager::addInputDeviceIfNotThere(const String & name)
{
	if (getInputDeviceWithName(name) != nullptr) return;
	MIDIInputDevice * d = new MIDIInputDevice(name);
	inputs.add(d);

	NLOG("MIDI", "Device In Added : " << name);

	listeners.call(&Listener::midiDeviceInAdded, d);
}

void MIDIManager::addOutputDeviceIfNotThere(const String & name)
{
	if (getOutputDeviceWithName(name) != nullptr) return;
	MIDIOutputDevice * d = new MIDIOutputDevice(name);
	outputs.add(d);

	NLOG("MIDI", "Device Out Added : " << name);

	listeners.call(&Listener::midiDeviceOutAdded, d);
}

void MIDIManager::removeInputDevice(MIDIInputDevice * d)
{
	inputs.removeObject(d,false);

	NLOG("MIDI", "Device In Removed : " << d->name);

	listeners.call(&Listener::midiDeviceInRemoved, d);
	delete d;
}

void MIDIManager::removeOutputDevice(MIDIOutputDevice * d)
{
	outputs.removeObject(d, false);
	
	NLOG("MIDI", "Device Out Removed : " << d->name);

	listeners.call(&Listener::midiDeviceOutRemoved, d);
	delete d;
}

MIDIInputDevice * MIDIManager::getInputDeviceWithName(const String & name)
{
	for (auto &d : inputs)
	{
		if (d->name == name) return d;
	}
	return nullptr;
}

MIDIOutputDevice * MIDIManager::getOutputDeviceWithName(const String & name)
{
	for (auto &d : outputs)
	{
		if (d->name == name) return d;
	}
	return nullptr;
}

void MIDIManager::timerCallback()
{
	checkDevices();
}