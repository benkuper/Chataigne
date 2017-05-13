/*
  ==============================================================================

    ModuleRouterManager.h
    Created: 29 Apr 2017 10:35:31am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEROUTERMANAGER_H_INCLUDED
#define MODULEROUTERMANAGER_H_INCLUDED


#include "ModuleRouter.h"

class ModuleRouterManager :
	public BaseManager<ModuleRouter>
{
public:
	juce_DeclareSingleton(ModuleRouterManager, true)

	ModuleRouterManager();
	~ModuleRouterManager();
};



#endif  // MODULEROUTERMANAGER_H_INCLUDED
