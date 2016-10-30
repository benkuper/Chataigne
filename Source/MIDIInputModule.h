/*
  ==============================================================================

    MIDIInputModule.h
    Created: 29 Oct 2016 7:08:56pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MIDIINPUTMODULE_H_INCLUDED
#define MIDIINPUTMODULE_H_INCLUDED


//Put "*Module" to avoid confusion with JUCE MIDI classes


#include "Input.h"

class MIDIInputModule :
	public Input
{
public:
	MIDIInputModule();
	~MIDIInputModule() {}

	static MIDIInputModule * create() { return new MIDIInputModule(); }
	virtual String getTypeString() const override { return "MIDI"; }
};


#endif  // MIDIINPUTMODULE_H_INCLUDED
