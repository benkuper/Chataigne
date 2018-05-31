/*
  ==============================================================================

    MappingOutputManager.cpp
    Created: 28 Oct 2016 8:11:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutputManager.h"
#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"
#include "ui/MappingOutputManagerEditor.h"

MappingOutputManager::MappingOutputManager() :
	BaseManager<MappingOutput>("Outputs"),
	outParam(nullptr),
	omAsyncNotifier(5)
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

void MappingOutputManager::setOutParam(Parameter * p)
{
	if (outParam == p) return;
	outParam = p;
	omAsyncNotifier.addMessage(new OutputManagerEvent(OutputManagerEvent::OUTPUT_CHANGED));
}

InspectableEditor * MappingOutputManager::getEditor(bool isRoot)
{
	return new MappingOutputManagerEditor(this, isRoot);
}
