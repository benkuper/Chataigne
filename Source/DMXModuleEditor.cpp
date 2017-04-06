/*
  ==============================================================================

    DMXModuleEditor.cpp
    Created: 6 Apr 2017 10:39:49pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModuleEditor.h"

DMXModuleEditor::DMXModuleEditor(DMXModule * module, bool isRoot) :
	ModuleEditor(module, isRoot),
	dmxModule(module)
{
}

DMXModuleEditor::~DMXModuleEditor()
{
}
