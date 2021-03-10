/*
  ==============================================================================

	ModuleRouterManager.cpp
	Created: 29 Apr 2017 10:35:31am
	Author:  Ben

  ==============================================================================
*/

juce_ImplementSingleton(ModuleRouterManager)

ModuleRouterManager::ModuleRouterManager() :
	BaseManager("Routers")
{
	
	selectItemWhenCreated = true;
}

ModuleRouterManager::~ModuleRouterManager()
{
}
