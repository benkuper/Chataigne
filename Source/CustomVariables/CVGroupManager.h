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
	public Manager<CVGroup>
{
public:
	juce_DeclareSingleton(CVGroupManager, true);

	CVGroupManager(const String &name = "Custom Variables");
	~CVGroupManager();

	std::unique_ptr<CustomVariablesModule> module;

	//Input values menu
	static void showMenuAndGetContainer(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc);
	static void showMenuAndGetVariable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, ControllableContainer* startFromCC, std::function<void(Controllable*)> returnFunc);
	static void showMenuAndGetPreset(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc);
	static void showMenuAndGetGroup(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc);
};