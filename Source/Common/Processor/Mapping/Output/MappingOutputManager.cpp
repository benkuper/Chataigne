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

MappingOutputManager::MappingOutputManager(IteratorProcessor * iterator) :
	BaseManager<MappingOutput>("Outputs"),
	IterativeTarget(iterator),
	omAsyncNotifier(5)
{
	canBeCopiedAndPasted = true;
	selectItemWhenCreated = false;
}

MappingOutputManager::~MappingOutputManager()
{
}


void MappingOutputManager::clear()
{
	for (auto& o : items) o->removeCommandHandlerListener(this);
	BaseManager::clear();
}

MappingOutput* MappingOutputManager::createItem()
{
	return new MappingOutput(iterator);
}

void MappingOutputManager::setOutParams(Array<Parameter *> params)
{
	outParams = Array<WeakReference<Parameter>>(params.getRawDataPointer(), params.size());
	if(outParams.size() > 0) for (auto &o : items) o->setOutputType(outParams[0]->type); //better than this ? should handle all ?

	omAsyncNotifier.addMessage(new OutputManagerEvent(OutputManagerEvent::OUTPUT_CHANGED));
}


void MappingOutputManager::updateOutputValues(int iterationIndex)
{
	var value = getMergedOutValue();
	if (value.isVoid()) return; //possible if parameters have been deleted in another thread during process

	for (auto& i : items) i->setValue(value, iterationIndex);
}

void MappingOutputManager::updateOutputValue(MappingOutput * o, int iterationIndex)
{
	if (outParams.size() == 0) return;
	if (o == nullptr) return;
	o->setValue(getMergedOutValue(), iterationIndex);
}

var MappingOutputManager::getMergedOutValue()
{
	var value;
	for (auto& o : outParams)
	{
		if (o.wasObjectDeleted()) return var();

		var val = o->getValue();
		if (!val.isArray()) value.append(val);
		else
		{
			for (int i = 0; i < val.size(); ++i)
			{
				value.append(val[i]);
			}
		}
	}

	return value;
}

void MappingOutputManager::addItemInternal(MappingOutput * o, var)
{
	o->addCommandHandlerListener(this);
	if(outParams.size() > 0)o->setOutputType(outParams[0]->type);
	for (int i = 0; i < getIterationCount(); i++) updateOutputValue(o, i);
}

void MappingOutputManager::removeItemInternal(MappingOutput * o)
{
	o->removeCommandHandlerListener(this);
}

void MappingOutputManager::commandChanged(BaseCommandHandler * h)
{
	for (int i = 0; i < getIterationCount(); i++) updateOutputValue(dynamic_cast<MappingOutput *>(h), i);
}

void MappingOutputManager::commandUpdated(BaseCommandHandler * h)
{
	for (int i = 0; i < getIterationCount(); i++) updateOutputValue(dynamic_cast<MappingOutput *>(h), i);
}

InspectableEditor * MappingOutputManager::getEditor(bool isRoot)
{
	return new MappingOutputManagerEditor(this, isRoot);
}
