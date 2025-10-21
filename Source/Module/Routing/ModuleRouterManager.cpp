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
	hideInRemoteControl = true;
	defaultHideInRemoteControl = true;

	selectItemWhenCreated = true;
}

ModuleRouterManager::~ModuleRouterManager()
{
}
