/*
  ==============================================================================

    MIDIDeviceParameterUI.h
    Created: 20 Dec 2016 3:06:05pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MIDIDEVICEPARAMETERUI_H_INCLUDED
#define MIDIDEVICEPARAMETERUI_H_INCLUDED

#include "../MIDIDeviceParameter.h"
#include "MIDIDeviceChooser.h"

class MIDIDeviceParameterUI :
	public ParameterUI,
	public MIDIDeviceChooser::ChooserListener
{
public:
	MIDIDeviceParameterUI(MIDIDeviceParameter * midiParam);
	~MIDIDeviceParameterUI();
	
	MIDIDeviceParameter * midiParam;
	MIDIDeviceChooser chooser;
	
	void resized() override;

	void valueChanged(const var &value) override;

	void midiDeviceInSelected(MIDIInputDevice * d) override;
	void midiDeviceOutSelected(MIDIOutputDevice * d) override;
};

#endif  // MIDIDEVICEPARAMETERUI_H_INCLUDED
