/*
  ==============================================================================

    ResolumeModuleEditor.h
    Created: 20 Nov 2017 5:34:59pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "ResolumeModule.h"
#include "OSCModuleBaseEditor.h"

class ResolumeModuleEditor :
	public OSCModuleBaseEditor
{
public:
	ResolumeModuleEditor(ResolumeModule * resolumeModule, bool isRoot);
	~ResolumeModuleEditor();

	ScopedPointer<EnumParameterUI> versionUI;

	void resizedOSCInternalContent(Rectangle<int> &r) override;

};