/*
  ==============================================================================

    DMXModuleEditor.h
    Created: 6 Apr 2017 10:39:49pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXMODULEEDITOR_H_INCLUDED
#define DMXMODULEEDITOR_H_INCLUDED

#include "ModuleEditor.h"
#include "DMXModule.h"

class DMXModuleEditor :
	public ModuleEditor
{
public:
	DMXModuleEditor(DMXModule * module, bool isRoot);
	~DMXModuleEditor();

	DMXModule * dmxModule;
};



#endif  // DMXMODULEEDITOR_H_INCLUDED
