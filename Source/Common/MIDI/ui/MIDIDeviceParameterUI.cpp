/*
  ==============================================================================

    MIDIDeviceParameterUI.cpp
    Created: 20 Dec 2016 3:06:05pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIDeviceParameterUI.h"

MIDIDeviceParameterUI::MIDIDeviceParameterUI(MIDIDeviceParameter * _midiParam) :
	ParameterUI(_midiParam),
	midiParam(_midiParam)
{
	addAndMakeVisible(&chooser);
	chooser.addMIDIChooserListener(this);
	chooser.setSelectedInputDevice(midiParam->inputDevice);
	chooser.setSelectedOutputDevice(midiParam->outputDevice);
	chooser.setGhostValues(midiParam->ghostDeviceIn, midiParam->ghostDeviceOut);
}

MIDIDeviceParameterUI::~MIDIDeviceParameterUI()
{
}

void MIDIDeviceParameterUI::resized()
{
	chooser.setBounds(getLocalBounds());
}

void MIDIDeviceParameterUI::valueChanged(const var & /*value*/)
{

	chooser.setSelectedInputDevice(midiParam->inputDevice);
	chooser.setSelectedOutputDevice(midiParam->outputDevice);
	chooser.setGhostValues(midiParam->ghostDeviceIn, midiParam->ghostDeviceOut);
}

void MIDIDeviceParameterUI::midiDeviceInSelected(MIDIInputDevice * d)
{
	midiParam->setInputDevice(d);
}

void MIDIDeviceParameterUI::midiDeviceOutSelected(MIDIOutputDevice * d)
{
	midiParam->setOutputDevice(d);
}
