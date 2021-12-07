/*
  ==============================================================================

    MIDIDeviceParameterUI.cpp
    Created: 20 Dec 2016 3:06:05pm
    Author:  Ben

  ==============================================================================
*/

MIDIDeviceParameterUI::MIDIDeviceParameterUI(Array<MIDIDeviceParameter *> _midiParams) :
	ParameterUI(Inspectable::getArrayAs<MIDIDeviceParameter, Parameter>(_midiParams)),
	midiParams(_midiParams),
	midiParam(_midiParams[0])
{
	addAndMakeVisible(&chooser);
	chooser.addMIDIChooserListener(this);
	chooser.setSelectedInputDevice(midiParam->inputDevice);
	chooser.setSelectedOutputDevice(midiParam->outputDevice);
	chooser.setGhostValues(midiParam->ghostDeviceIn, midiParam->ghostDeviceOut);

	setSize(100, 40);
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
	chooser.setGhostValues(midiParam->ghostDeviceNameIn, midiParam->ghostDeviceNameOut);
}

void MIDIDeviceParameterUI::midiDeviceInSelected(MIDIInputDevice * d)
{
	midiParam->setInputDevice(d);
}

void MIDIDeviceParameterUI::midiDeviceOutSelected(MIDIOutputDevice * d)
{
	midiParam->setOutputDevice(d);
}
