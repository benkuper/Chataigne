/*
  ==============================================================================

    MIDIModuleEditor.h
    Created: 20 Dec 2016 12:35:45pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MIDIMODULEEDITOR_H_INCLUDED
#define MIDIMODULEEDITOR_H_INCLUDED

#include "ModuleEditor.h"
#include "MIDIModule.h"
#include "MIDIDeviceChooser.h"

#include "MIDIDeviceParameterUI.h"


class MIDIModuleEditor :
	public ModuleEditor
{
public:
	MIDIModuleEditor(MIDIModule * _module, bool isRoot);
	~MIDIModuleEditor();

	MIDIModule * midiModule;
	
	ScopedPointer<ControllableEditor> deviceChooser;
	ScopedPointer<BoolToggleUI> autoAddUI;
	ScopedPointer<InspectableEditor> valuesUI;

	void resizedInternalContent(Rectangle<int> &r) override;
};




#endif  // MIDIMODULEEDITOR_H_INCLUDED
