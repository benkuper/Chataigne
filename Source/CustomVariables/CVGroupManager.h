/*
  ==============================================================================

    CVGroupManager.h
    Created: 17 Feb 2018 10:16:11am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "CVGroup.h"

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
	static ControllableContainer * showMenuAndGetContainer();
	static Controllable * showMenuAndGetVariable(bool, bool);
	static ControllableContainer * showMenuAndGetPreset();
};