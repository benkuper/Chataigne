/*
  ==============================================================================

    MIDIDeviceChooser.cpp
    Created: 20 Dec 2016 12:35:11pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIDeviceChooser.h"

MIDIDeviceChooser::MIDIDeviceChooser(bool _showInputs, bool _showOutputs) :
showInputs(_showInputs),
showOutputs(_showOutputs),
currentInputDevice(nullptr),
	currentOutputDevice(nullptr)
{
	if (showInputs)
	{
		//addAndMakeVisible(&inputLabel);
		addAndMakeVisible(&inputBox);
		
		
	}

	if (showOutputs)
	{
		//addAndMakeVisible(&outputLabel);
		addAndMakeVisible(&outputBox);
		
	}


	setGhostValues("", "");

	inputBox.addListener(this);
	outputBox.addListener(this);

	MIDIManager::getInstance()->addMIDIManagerListener(this);

	updateInputComboBox();
	updateOutputComboBox();
}

MIDIDeviceChooser::~MIDIDeviceChooser()
{
	if (MIDIManager::getInstanceWithoutCreating() != nullptr)
	{
		MIDIManager::getInstance()->removeMIDIManagerListener(this);
	}
}

void MIDIDeviceChooser::resized()
{
	Rectangle<int> r = getLocalBounds();
	int th = (showInputs != showOutputs)?getHeight():getHeight() / 2 - 2;

	if (showInputs)
	{
		Rectangle<int> ir = r.removeFromTop(th);
		//inputLabel.setBounds(ir.removeFromLeft(50));
		//r.removeFromLeft(10);
		inputBox.setBounds(ir);

		r.removeFromTop(4);
	}

	if (showOutputs)
	{
		Rectangle<int> outR = r.removeFromTop(th);
		//outputLabel.setBounds(outR.removeFromLeft(50));
		//r.removeFromLeft(10);
		outputBox.setBounds(outR);
	}
}

void MIDIDeviceChooser::setGhostValues(const String & inValue, const String & outValue)
{
	inputBox.setTextWhenNoChoicesAvailable(inValue.isEmpty() ? "No MIDI In Available" : "Disconnected : " + inValue);
	outputBox.setTextWhenNoChoicesAvailable(outValue.isEmpty() ? "No MIDI Out Available" : "Disconnected : " + outValue);

	inputBox.setTextWhenNothingSelected(inValue.isEmpty() ? "No MIDI In Selected" : "Disconnected : " + inValue);
	outputBox.setTextWhenNothingSelected(outValue.isEmpty() ? "No MIDI Out Selected" : "Disconnected : " + outValue);
}

void MIDIDeviceChooser::updateInputComboBox()
{
	inputBox.clear(dontSendNotification);
	int index = 1;
	int idToSelect = 0;
	inputBox.addItem("Don't use input", -1);
	for (auto &i : MIDIManager::getInstance()->inputs)
	{
		if (currentInputDevice == i) idToSelect = index;
		inputBox.addItem(i->name, index);
		index++;
	}

	inputBox.setSelectedId(idToSelect, dontSendNotification);
}

void MIDIDeviceChooser::updateOutputComboBox()
{
	outputBox.clear(dontSendNotification);
	int index = 1;
	int idToSelect = 0;
	outputBox.addItem("Don't use output", -1);
	for (auto &o : MIDIManager::getInstance()->outputs)
	{
		if (currentOutputDevice == o) idToSelect = index;
		outputBox.addItem(o->name, index);
		index++;
	}

	outputBox.setSelectedId(idToSelect, dontSendNotification);
}

void MIDIDeviceChooser::setSelectedInputDevice(MIDIInputDevice * i)
{
	inputBox.setSelectedId(MIDIManager::getInstance()->inputs.indexOf(i)+1, dontSendNotification);
}

void MIDIDeviceChooser::setSelectedInputDevice(const String & deviceName)
{
	setSelectedInputDevice(MIDIManager::getInstance()->getInputDeviceWithName(deviceName));
}

void MIDIDeviceChooser::setSelectedOutputDevice(MIDIOutputDevice * o)
{
	outputBox.setSelectedId(MIDIManager::getInstance()->outputs.indexOf(o) + 1, dontSendNotification);
}

void MIDIDeviceChooser::setSelectedOutputDevice(const String & deviceName)
{
	setSelectedOutputDevice(MIDIManager::getInstance()->getOutputDeviceWithName(deviceName));
}

void MIDIDeviceChooser::comboBoxChanged(ComboBox * cb)
{
	int deviceIndex = cb->getSelectedId() - 1;

	if (cb == &inputBox)
	{
		currentInputDevice = deviceIndex >= 0 ? MIDIManager::getInstance()->inputs[deviceIndex] : nullptr;
		chooserListeners.call(&ChooserListener::midiDeviceInSelected, currentInputDevice);
	}
	else if (cb == &outputBox)
	{
		currentOutputDevice = deviceIndex >= 0 ? MIDIManager::getInstance()->outputs[deviceIndex] : nullptr;
		chooserListeners.call(&ChooserListener::midiDeviceOutSelected, currentOutputDevice);
	}

}

void MIDIDeviceChooser::midiDeviceInAdded(MIDIInputDevice *)
{
	updateInputComboBox();
}

void MIDIDeviceChooser::midiDeviceOutAdded(MIDIOutputDevice *)
{
	updateOutputComboBox();
}

void MIDIDeviceChooser::midiDeviceInRemoved(MIDIInputDevice *)
{
	updateInputComboBox();
}

void MIDIDeviceChooser::midiDeviceOutRemoved(MIDIOutputDevice *)
{
	updateOutputComboBox();
}
