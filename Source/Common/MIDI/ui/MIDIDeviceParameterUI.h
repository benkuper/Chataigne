/*
  ==============================================================================

    MIDIDeviceParameterUI.h
    Created: 20 Dec 2016 3:06:05pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class MIDIDeviceParameterUI :
	public ParameterUI,
	public MIDIDeviceChooser::ChooserListener
{
public:
	MIDIDeviceParameterUI(Array<MIDIDeviceParameter *> midiParam);
	~MIDIDeviceParameterUI();
	
	Array<MIDIDeviceParameter *> midiParams;
	MIDIDeviceParameter* midiParam;
	MIDIDeviceChooser chooser;
	
	void resized() override;

	void valueChanged(const var &value) override;

	void midiDeviceInSelected(MIDIInputDevice * d) override;
	void midiDeviceOutSelected(MIDIOutputDevice * d) override;
};