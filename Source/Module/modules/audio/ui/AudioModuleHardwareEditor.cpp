/*
  ==============================================================================

    AudioModuleHardwareEditor.cpp
    Created: 4 Jan 2018 1:45:32pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioModuleHardwareEditor.h"

AudioModuleHardwareEditor::AudioModuleHardwareEditor(AudioModuleHardwareSettings * hs, bool isRoot) :
	GenericControllableContainerEditor(hs,isRoot),
	hs(hs),
	selector(*hs->am, 0, 16, 0, 16, false, false, false, false)
{
	addAndMakeVisible(selector);
	selector.setSize(100, 300);
	selector.setVisible(!container->editorIsCollapsed);
}

AudioModuleHardwareEditor::~AudioModuleHardwareEditor()
{
}

void AudioModuleHardwareEditor::setCollapsed(bool value, bool force, bool animate)
{
	GenericControllableContainerEditor::setCollapsed(value, force, animate);
	selector.setVisible(!container->editorIsCollapsed);
}

void AudioModuleHardwareEditor::resizedInternalContent(Rectangle<int>& r)
{
	selector.setBounds(r.withHeight(selector.getHeight()));
	r.setY(selector.getHeight());
	r.setHeight(0);
}
