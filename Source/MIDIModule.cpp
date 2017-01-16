/*
  ==============================================================================

    MIDIModule.cpp
    Created: 20 Dec 2016 12:35:26pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIModule.h"
#include "MIDICommands.h"

MIDIModule::MIDIModule(const String & name) :
	Module(name),
	inputDevice(nullptr),
	outputDevice(nullptr)
{
	
	midiParam = new MIDIDeviceParameter("Devices");
	addParameter(midiParam);

	autoAdd = addBoolParameter("Auto Add", "Auto Add MIDI values that are received but not in the list", false);

	defManager.add(CommandDefinition::createDef(this, "", "Note On", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_ON));
	defManager.add(CommandDefinition::createDef(this, "", "Note Off", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::NOTE_OFF));
	defManager.add(CommandDefinition::createDef(this, "", "Controller Change", &MIDINoteAndCCCommand::create)->addParam("type", (int)MIDINoteAndCCCommand::CONTROLCHANGE));
}

MIDIModule::~MIDIModule()
{
	if(inputDevice != nullptr) inputDevice->removeMIDIInputListener(this);
	if (outputDevice != nullptr) outputDevice->close();
}


void MIDIModule::sendNoteOn(int pitch, int velocity, int channel)
{
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note on " << MIDIManager::getNoteName(pitch) << ", " << velocity << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOn(pitch, velocity, channel);
}

void MIDIModule::sendNoteOff(int pitch, int channel)
{
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Note off " << MIDIManager::getNoteName(pitch) << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendNoteOff(pitch, channel);
}

void MIDIModule::sendControlChange(int number, int value, int channel)
{
	if (outputDevice == nullptr) return;
	if (logOutgoingData->boolValue()) NLOG(niceName, "Send Control Change " << number << ", " << value << ", " << channel);
	outActivityTrigger->trigger();
	outputDevice->sendControlChange(number,value,channel);
}

void MIDIModule::onContainerParameterChangedInternal(Parameter * p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == midiParam)
	{
		updateMIDIDevices();
	}
}

void MIDIModule::updateMIDIDevices()
{
	MIDIInputDevice * newInput = midiParam->inputDevice;
	if (inputDevice != newInput)
	{
		if (inputDevice != nullptr)
		{
			inputDevice->removeMIDIInputListener(this);
		}
		inputDevice = newInput;
		if (inputDevice != nullptr)
		{
			inputDevice->addMIDIInputListener(this);
		}
	}

	MIDIOutputDevice * newOutput = midiParam->outputDevice;
	if (outputDevice != newOutput)
	{
		if(outputDevice != nullptr) outputDevice->close();
		outputDevice = newOutput;
		if(outputDevice != nullptr) outputDevice->open();
	}
}

void MIDIModule::noteOnReceived(const int & channel, const int & pitch, const int & velocity)
{
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue())  NLOG(niceName, "Note On : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	updateValue(MIDIManager::getNoteName(pitch) + " (" + String(channel) + ")", velocity);
}

void MIDIModule::noteOffReceived(const int & channel, const int & pitch, const int & velocity)
{
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Note Off : " << channel << ", " << MIDIManager::getNoteName(pitch) << ", " << velocity);

	updateValue(MIDIManager::getNoteName(pitch) + " (" + String(channel) + ")", velocity);
	
}

void MIDIModule::controlChangeReceived(const int & channel, const int & number, const int & value)
{
	inActivityTrigger->trigger();
	if (logIncomingData->boolValue()) NLOG(niceName, "Control Change : " << channel << ", " << number << ", " << value);

	updateValue("CC"+String(number)+" ("+String(channel)+")", value);

}

void MIDIModule::updateValue(const String & n, const int & val)
{

	Parameter * p = dynamic_cast<Parameter *>(valuesCC.getControllableByName(n,true));
	if (p == nullptr)
	{
		if (autoAdd->boolValue())
		{
			p = valuesCC.addIntParameter(n, n, val, 0, 127);
			p->isRemovableByUser = true;
			p->saveValueOnly = false;
			valuesCC.orderControllablesAlphabetically();
		}
	}
	else
	{
		p->setValue(val);
	}

}

var MIDIModule::getJSONData()
{
	var data = Module::getJSONData();
	data.getDynamicObject()->setProperty("values", valuesCC.getJSONData());
	return data;
}

void MIDIModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	DBG("Values cc load with create if not there");
	valuesCC.loadJSONData(data.getProperty("values", var()), true);
	valuesCC.orderControllablesAlphabetically();
}
