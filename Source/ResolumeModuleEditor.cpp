/*
  ==============================================================================

    ResolumeModuleEditor.cpp
    Created: 20 Nov 2017 5:34:59pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeModuleEditor.h"

ResolumeModuleEditor::ResolumeModuleEditor(ResolumeModule * resolumeModule, bool isRoot) :
	OSCModuleBaseEditor(resolumeModule,isRoot)
{
	versionUI = resolumeModule->version->createUI();
	addAndMakeVisible(versionUI);
}

ResolumeModuleEditor::~ResolumeModuleEditor()
{
}

void ResolumeModuleEditor::resizedOSCInternalContent(Rectangle<int>& r)
{
	versionUI->setBounds(r.withHeight(14));
	r.setBottom(versionUI->getBottom());
}
