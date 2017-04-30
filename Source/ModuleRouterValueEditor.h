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

#include "BaseItemUI.h"
#include "ModuleRouterValue.h"

class ModuleRouterValueEditor : 
	public BaseItemUI<ModuleRouterValue>
{
public:
	ModuleRouterValueEditor(ModuleRouterValue * mrv);
	~ModuleRouterValueEditor();
};

#endif