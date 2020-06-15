/*
  ==============================================================================

    ModuleRouterController.h
    Created: 15 Jun 2020 5:34:44pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ModuleRouter;
class ModuleRouterControllerUI;

class ModuleRouterController :
	public ControllableContainer
{
public:
	ModuleRouterController(ModuleRouter* router) :
		ControllableContainer("Controller"), router(router) {}
	virtual ~ModuleRouterController() {}

	ModuleRouter* router;

	virtual ModuleRouterControllerUI* getUI() { return nullptr; }
};