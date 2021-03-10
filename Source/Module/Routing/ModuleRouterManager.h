/*
  ==============================================================================

    ModuleRouterManager.h
    Created: 29 Apr 2017 10:35:31am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterManager :
	public BaseManager<ModuleRouter>
{
public:
	juce_DeclareSingleton(ModuleRouterManager, true)

	ModuleRouterManager();
	~ModuleRouterManager();
};