/*
  ==============================================================================

    CustomOSCModuleEditor.cpp
    Created: 27 Dec 2016 1:54:47pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCModuleEditor.h"

CustomOSCModuleEditor::CustomOSCModuleEditor(CustomOSCModule * _cModule, bool isRoot) :
	OSCModuleBaseEditor(_cModule,isRoot),
	cModule(_cModule)
{
	autoAddUI = cModule->autoAdd->createToggle();
	addAndMakeVisible(autoAddUI);

	valuesEditor = cModule->valuesCC.getEditor(false);
	addAndMakeVisible(valuesEditor);
}

CustomOSCModuleEditor::~CustomOSCModuleEditor()
{
}

int CustomOSCModuleEditor::resizedInternal(Rectangle<int> r)
{
	autoAddUI->setBounds(r.withHeight(14));
	valuesEditor->setBounds(r.translated(0,16));
	return r.withBottom(valuesEditor->getBottom()).getHeight();
}
