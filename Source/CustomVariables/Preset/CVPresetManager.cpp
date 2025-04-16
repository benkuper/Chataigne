/*
  ==============================================================================

    CVPresetManager.cpp
    Created: 22 Feb 2018 4:10:48pm
    Author:  Ben

  ==============================================================================
*/

CVPresetManager::CVPresetManager(CVGroup * group) :
	Manager("Presets"),
	group(group)
{
	selectItemWhenCreated = false;
	canBeCopiedAndPasted = true;
}

CVPresetManager::~CVPresetManager()
{

}

CVPreset * CVPresetManager::createItem()
{
	return new CVPreset(group);
}
