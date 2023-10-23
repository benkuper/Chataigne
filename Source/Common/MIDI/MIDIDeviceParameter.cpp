/*
  ==============================================================================

	MIDIDeviceParameter.cpp
	Created: 20 Dec 2016 3:05:54pm
	Author:  Ben

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

MIDIDeviceParameter::MIDIDeviceParameter(const String& name, const String& description) :
	Parameter(CUSTOM, name, "MIDI Devices", description, var(), var()),
	inputDevice(nullptr),
	outputDevice(nullptr)
{
	MIDIManager::getInstance()->addMIDIManagerListener(this);
	value.append("");
	value.append("");
}

MIDIDeviceParameter::~MIDIDeviceParameter()
{
	if (MIDIManager::getInstanceWithoutCreating() != nullptr)
	{
		MIDIManager::getInstance()->removeMIDIManagerListener(this);
	}
}



void MIDIDeviceParameter::setInputDevice(MIDIInputDevice* i)
{
	var val;
	val.append(i != nullptr ? i->id : "");
	val.append(value[1]);

	if (i != nullptr)
	{
		ghostDeviceIn = value[0];
		ghostDeviceNameIn = i->name;
	}

	inputDevice = i;

	setValue(val);
}

void MIDIDeviceParameter::setOutputDevice(MIDIOutputDevice* o)
{
	var val;
	val.append(value[0]);
	val.append(o != nullptr ? o->id : "");

	if (o != nullptr)
	{
		ghostDeviceOut = val[1];
		ghostDeviceNameOut = o->name;
	}

	outputDevice = o;

	setValue(val, false, true);

}

void MIDIDeviceParameter::midiDeviceInAdded(MIDIInputDevice* i)
{
	//DBG("Device In added " << i->name << " / " << ghostDeviceIn);
	if (inputDevice == nullptr && i->id == ghostDeviceIn)
	{
		setInputDevice(i);
	}
}

void MIDIDeviceParameter::midiDeviceOutAdded(MIDIOutputDevice* o)
{
	if (outputDevice == nullptr && o->id == ghostDeviceOut)
	{
		setOutputDevice(o);
	}
}

void MIDIDeviceParameter::midiDeviceInRemoved(MIDIInputDevice* i)
{
	if (i == inputDevice)
	{
		if (i != nullptr)
		{
			ghostDeviceIn = i->id;
			ghostDeviceNameIn = i->name;
		}
		setInputDevice(nullptr);
	}
}

void MIDIDeviceParameter::midiDeviceOutRemoved(MIDIOutputDevice* o)
{
	if (o == outputDevice)
	{
		if (o != nullptr)
		{
			ghostDeviceOut = o->id;
			ghostDeviceNameOut = o->name;
		}

		setOutputDevice(nullptr);
	}
}


MIDIDeviceParameterUI* MIDIDeviceParameter::createMIDIParameterUI(Array<MIDIDeviceParameter*> parameters)
{
	if (parameters.size() == 0) parameters = { this };
	return new MIDIDeviceParameterUI(parameters);
}

ControllableUI* MIDIDeviceParameter::createDefaultUI(Array<Controllable*> controllables)
{
	Array<MIDIDeviceParameter*> parameters = Inspectable::getArrayAs<Controllable, MIDIDeviceParameter>(controllables);
	if (parameters.size() == 0) parameters.add(this);
	return createMIDIParameterUI(parameters);
}

void MIDIDeviceParameter::loadJSONDataInternal(var data)
{
	Parameter::loadJSONDataInternal(data);
	setInputDevice(MIDIManager::getInstance()->getInputDeviceWithID(value[0]));

	if (inputDevice == nullptr) ghostDeviceIn = data.getProperty("value", var())[0];

	setOutputDevice(MIDIManager::getInstance()->getOutputDeviceWithID(value[1]));

	if (outputDevice == nullptr) ghostDeviceOut = data.getProperty("value", var())[1];

}
