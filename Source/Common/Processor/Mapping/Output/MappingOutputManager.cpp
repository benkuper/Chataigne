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
	omAsyncNotifier(5)
{
	selectItemWhenCreated = false;
}

MappingOutputManager::~MappingOutputManager()
{
}


void MappingOutputManager::setOutParams(Array<Parameter *> params)
{
	outParams = params;
	if(outParams.size() > 0) for (auto &o : items) o->setOutputType(outParams[0]->type); //better than this ? should handle all ?

	omAsyncNotifier.addMessage(new OutputManagerEvent(OutputManagerEvent::OUTPUT_CHANGED));
}


void MappingOutputManager::updateOutputValues()
{
	var value = getMergedOutValue();
	for (auto & i : items) i->setValue(value);
}

void MappingOutputManager::updateOutputValue(MappingOutput * o)
{
	if (outParams.size() == 0) return;
	if (o == nullptr) return;
	o->setValue(getMergedOutValue());
}

var MappingOutputManager::getMergedOutValue()
{
	var value;
	for (auto& o : outParams)
	{
		var val = o->getValue();
		if (!val.isArray()) value.append(val);
		else
		{
			for (int i = 0; i < val.size(); i++)
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
