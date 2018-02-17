/*
  ==============================================================================

    CustomVariable.h
    Created: 17 Feb 2018 10:15:09am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CustomVariable :
	public BaseItem
{
public:
	CustomVariable(const String &name = "Custom Variable", var params = var());
	~CustomVariable();

	Parameter * parameter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomVariable)
};