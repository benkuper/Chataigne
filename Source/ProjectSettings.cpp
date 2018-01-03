/*
  ==============================================================================

    ProjectSettings.cpp
    Created: 22 Nov 2017 11:30:40am
    Author:  Ben

  ==============================================================================
*/

#include "ProjectSettings.h"

juce_ImplementSingleton(ProjectSettings)

ProjectSettings::ProjectSettings() :
	ControllableContainer("Project Settings")
{
	saveAndLoadRecursiveData = true;
}

ProjectSettings::~ProjectSettings()
{
}
