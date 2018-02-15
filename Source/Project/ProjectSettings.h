/*
  ==============================================================================

    ProjectSettings.h
    Created: 22 Nov 2017 11:30:40am
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ProjectSettings :
	public ControllableContainer
{
public:
	juce_DeclareSingleton(ProjectSettings, true);
	ProjectSettings();
	~ProjectSettings();
};