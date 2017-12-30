/*
  ==============================================================================

    OSCModuleIOEditor.h
    Created: 30 Dec 2017 7:47:46pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "OSCModule.h"

class OSCIOEditor :
	public GenericControllableContainerEditor
{
public:
	OSCIOEditor(OSCIOContainer * cc, bool isRoot);
	~OSCIOEditor() {}

	OSCIOContainer * ioContainer;
	ScopedPointer<BoolImageToggleUI> enabledUI;

	void resizedInternalHeader(Rectangle<int> &r) override;
};