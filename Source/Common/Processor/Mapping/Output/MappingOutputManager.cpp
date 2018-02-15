/*
  ==============================================================================

    MappingOutputManager.cpp
    Created: 28 Oct 2016 8:11:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutputManager.h"
#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"

MappingOutputManager::MappingOutputManager() :
	BaseManager<MappingOutput>("Outputs")
{
	selectItemWhenCreated = false;
}

MappingOutputManager::~MappingOutputManager()
{
}

void MappingOutputManager::setValue(var value)
{
	for (auto &o : items)
	{
		o->setValue(value);
	}
}

InspectableEditor * MappingOutputManager::getEditor(bool isRoot)
{
	return new BaseCommandHandlerManagerEditor<MappingOutput>(this, CommandContext::MAPPING, isRoot);
}
