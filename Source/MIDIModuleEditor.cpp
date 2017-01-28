/*
  ==============================================================================

    MIDIModuleEditor.cpp
    Created: 20 Dec 2016 12:35:45pm
    Author:  Ben

  ==============================================================================
*/

#include "MIDIModuleEditor.h"

MIDIModuleEditor::MIDIModuleEditor(MIDIModule * _module, bool isRoot) :
	ModuleEditor(_module,isRoot),
	midiModule(_module)
{
	deviceChooser = (ControllableEditor *)midiModule->midiParam->getEditor(false);
	autoAddUI = midiModule->autoAdd->createToggle();
	valuesUI = midiModule->valuesCC.getEditor(false);

	addAndMakeVisible(deviceChooser);
	addAndMakeVisible(autoAddUI);
	addAndMakeVisible(valuesUI);
}

MIDIModuleEditor::~MIDIModuleEditor()
{
}

void MIDIModuleEditor::resizedInternalContent(Rectangle<int>& r)
{
	deviceChooser->setBounds(r.withHeight(40));
	r.translate(0,deviceChooser->getHeight() + 2);
	autoAddUI->setBounds(r.withSize(100,16));
	r.translate(0, autoAddUI->getHeight() + 2);
	r.setHeight(valuesUI->getHeight());
	valuesUI->setBounds(r);
}
