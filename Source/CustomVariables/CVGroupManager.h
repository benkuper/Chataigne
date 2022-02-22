/*
  ==============================================================================

    CVGroupManager.h
    Created: 17 Feb 2018 10:16:11am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CustomVariablesModule;

class CVGroupManager :
	public BaseManager<CVGroup>
{
public:
	juce_DeclareSingleton(CVGroupManager, true);

	CVGroupManager(const String &name = "Custom Variables");
	~CVGroupManager();

	std::unique_ptr<CustomVariablesModule> module;

	//Input values menu
	static void showMenuAndGetContainer(std::function<void(ControllableContainer*)> returnFunc);
	static void showMenuAndGetVariable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, std::function<void(Controllable*)> returnFunc);
	static void showMenuAndGetPreset(std::function<void(ControllableContainer*)> returnFunc);
	static void showMenuAndGetGroup(std::function<void(ControllableContainer*)> returnFunc);
};