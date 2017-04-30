/*
  ==============================================================================

    ModuleRouterValueEditor.h
    Created: 30 Apr 2017 2:41:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#ifndef MODULEROUTERVALUEDITOR_H_INCLUDED
#define MODULEROUTERVALUEDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "Module.h"

class ModuleRouterValueEditor : 
	public Component
{
public:
	ModuleRouterValueEditor(Module * sourceModule, Controllable * sourceValue, Module * outputModule);
	~ModuleRouterValueEditor();

	ScopedPointer<InspectableEditor> valueEditor;
	ScopedPointer<BoolToggleUI> valueIsUsed;

	ScopedPointer<RouterOutput> output;
};

#endif