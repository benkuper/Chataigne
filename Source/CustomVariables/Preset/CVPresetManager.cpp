/*
  ==============================================================================

    CVPresetManager.cpp
    Created: 22 Feb 2018 4:10:48pm
    Author:  Ben

  ==============================================================================
*/

#include "CVPresetManager.h"
#include "../CVGroup.h"

CVPresetManager::CVPresetManager(CVGroup * group) :
	BaseManager("Presets"),
	group(group)
{
	selectItemWhenCreated = false;
}

CVPresetManager::~CVPresetManager()
{

}

CVPreset * CVPresetManager::createItem()
{
	return new CVPreset(group);
}
