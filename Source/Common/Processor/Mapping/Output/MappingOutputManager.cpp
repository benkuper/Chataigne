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
	for (auto &o : items) o->setValue(value);
}

void MappingOutputManager::setOutParam(Parameter * p)
{
	if (outParam == p) return;
	outParam = p;
	for (auto &o : items) o->setOutputType(outParam->type);

	omAsyncNotifier.addMessage(new OutputManagerEvent(OutputManagerEvent::OUTPUT_CHANGED));
}

void MappingOutputManager::updateOutputValue(MappingOutput * o)
{
	if (outParam == nullptr) return;
	if (o == nullptr) return;
	o->setValue(outParam->value);
}

void MappingOutputManager::addItemInternal(MappingOutput * o, var)
{
	o->adCommandHandlerListener(this);
	if(outParam != nullptr) o->setOutputType(outParam->type);
	updateOutputValue(o);
}

void MappingOutputManager::removeItemInternal(MappingOutput * o)
{
	o->removeCommandHandlerListener(this);
}

void MappingOutputManager::commandChanged(BaseCommandHandler * h)
{
	updateOutputValue(dynamic_cast<MappingOutput *>(h));
}

void MappingOutputManager::commandUpdated(BaseCommandHandler * h)
{
	updateOutputValue(dynamic_cast<MappingOutput *>(h));
}

InspectableEditor * MappingOutputManager::getEditor(bool isRoot)
{
	return new MappingOutputManagerEditor(this, isRoot);
}
