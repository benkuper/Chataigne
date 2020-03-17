/*
  ==============================================================================

    MappingInputManager.cpp
    Created: 1 Mar 2020 3:10:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingInputManager.h"

MappingInputManager::MappingInputManager() :
	BaseManager("Inputs")
{
	selectItemWhenCreated = false;
}

MappingInputManager::~MappingInputManager()
{
}

void MappingInputManager::lockInput(Array<Parameter*> input)
{
	clear();
	for (auto& i : input)
	{
		if (i == nullptr)  continue;
		MappingInput* mi = addItem();
		mi->lockInput(i);
	}
}

Array<Parameter *> MappingInputManager::getInputReferences()
{
	Array<Parameter *> result;
	for (auto& i : items)
	{
		if (i == nullptr || i->inputReference == nullptr) continue;
		result.add(i->inputReference);
	}
	return result;
}
