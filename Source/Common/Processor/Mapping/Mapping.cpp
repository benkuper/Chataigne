/*
  ==============================================================================

	Mapping.cpp
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"
#include "ui/MappingUI.h"

Mapping::Mapping(var params, Multiplex * multiplex, bool canBeDisabled) :
	Processor("Mapping", canBeDisabled),
	MultiplexTarget(multiplex),
	Thread("Mapping"),
	processMode(VALUE_CHANGE),
	mappingParams("Parameters"),
	im(multiplex),
	fm(multiplex),
	om(multiplex),
	outValuesCC("Out Values"),
	isRebuilding(false),
	isProcessing(false),
	shouldRebuildAfterProcess(false),
    inputIsLocked(false),
    mappingNotifier(10)
{
	itemDataType = "Mapping";
	type = MAPPING;
	
	updateRate = mappingParams.addIntParameter("Update rate", "This is the update rate at which the mapping is processing. This is used only when continuous filters like Smooth and Damping are presents", 50, 1, 500, false);
	//updateRate->canBeDisabledByUser = true;

	addChildControllableContainer(&im);
	addChildControllableContainer(&mappingParams);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);
	
	outValuesCC.hideInEditor = true;
	addChildControllableContainer(&outValuesCC);

	fm.addFilterManagerListener(this);
	im.addBaseManagerListener(this);

	helpID = "Mapping";
}


Mapping::~Mapping()
{
	stopThread(100);
	clearItem();
}

void Mapping::setProcessMode(ProcessMode mode)
{
	if (mode == processMode) return;
	processMode = mode;

	switch (processMode)
	{
	case VALUE_CHANGE:
		break;

	case MANUAL:
		break;

	case TIMER:
		checkFiltersNeedContinuousProcess();
		break;
	}
}

void Mapping::lockInputTo(Array<Parameter *> lockParams)
{
	inputIsLocked = lockParams.size() > 0;
	im.lockInput(lockParams);
	im.hideInEditor = inputIsLocked;
}

void Mapping::checkFiltersNeedContinuousProcess()
{
	bool need = false;
	if (processMode == TIMER) need = true;
	
	if (!need)
	{
		for (auto &f : fm.items)
		{
			if (f->processOnSameValue)
			{
				need = true;
				break;
			}
		}
	}

	updateRate->setEnabled(need);
}

void Mapping::updateMappingChain(MappingFilter * afterThisFilter)
{
	if (isCurrentlyLoadingData || isClearing) return;
	if (isRebuilding) return;

	if (isProcessing)
	{
		shouldRebuildAfterProcess = true;
		return;
	}

	{
		//enter in scope for lock
		GenericScopedLock lock(mappingLock);
		isRebuilding = true;

		if(afterThisFilter == nullptr) fm.setupSources(im.getInputReferences(0)); //do the whole rebuild
		else fm.rebuildFilterChain(afterThisFilter); //only ask to rebuild after the changed filter

		Array<Parameter*> processedParams = fm.getLastFilteredParameters();

		outValuesCC.clear();
		Array<Parameter*> outP;
		for (auto& sp : processedParams)
		{
			Parameter* p = ControllableFactory::createParameterFrom(sp, false, false);
			outP.add(p);
			outValuesCC.addParameter(p);
			p->setControllableFeedbackOnly(true);
			p->setNiceName("Out " + String(outP.size()));
			p->setValue(sp->value);
		}

		om.setOutParams(outP);

		mappingNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));

		checkFiltersNeedContinuousProcess();

		isRebuilding = false;
	}


	process();
}

void Mapping::process(bool forceOutput, int multiplexIndex)
{
	if ((canBeDisabled && (enabled != nullptr && !enabled->boolValue())) || forceDisabled) return;
	if (im.items.size() == 0) return;
	if (isCurrentlyLoadingData || isRebuilding || isProcessing || isClearing) return;

	//DBG("[PROCESS] Enter lock");
	{
		GenericScopedLock lock(mappingLock);
		ScopedLock filterLock(fm.filterLock);
		
		isProcessing = true;
		bool filterResult = fm.processFilters(multiplexIndex);

		if (filterResult)
		{
			for (int i = 0; i < fm.filteredParameters.size(); i++)
			{
				if (Parameter* fp = fm.filteredParameters[i])
				{
					if (Parameter* p = (Parameter*)outValuesCC.controllables[i])
					{
						if (p->type == Parameter::ENUM) ((EnumParameter*)p)->setValueWithKey(((EnumParameter*)fp)->getValueKey());
						else p->setValue(fp->value);
					}
				}
			}

			om.updateOutputValues(multiplexIndex);
		}
		
		isProcessing = false;
	}

	if (shouldRebuildAfterProcess)
	{
		shouldRebuildAfterProcess = false;
		updateMappingChain();
	}

	//DBG("[PROCESS] Exit lock");

}

var Mapping::getJSONData()
{
	var data = Processor::getJSONData();
	data.getDynamicObject()->setProperty("im", im.getJSONData());
	data.getDynamicObject()->setProperty("params", mappingParams.getJSONData()); //keep "params" to avoid conflict with ControllableContainer "parameters" object
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	if(!inputIsLocked) im.loadJSONData(data.getProperty("im", var()));
	mappingParams.loadJSONData(data.getProperty("params", var()));
	fm.loadJSONData(data.getProperty("filters", var()));
	om.loadJSONData(data.getProperty("outputs", var()));

	updateMappingChain();
}

void Mapping::afterLoadJSONDataInternal()
{
	updateMappingChain();
}

void Mapping::itemAdded(MappingInput* item)
{
	item->addMappingInputListener(this);
	if(item->inputReference != nullptr) updateMappingChain();
}

void Mapping::itemRemoved(MappingInput* item)
{
	item->removeMappingInputListener(this);
	updateMappingChain();
}

void Mapping::itemsReordered()
{
	updateMappingChain();
}

void Mapping::inputReferenceChanged(MappingInput *)
{
	if (Engine::mainEngine->isClearing) return;

	updateMappingChain();
}

void Mapping::inputParameterValueChanged(MappingInput *, int multiplexIndex)
{
	if (processMode == VALUE_CHANGE)
	{
		if (multiplexIndex == -1)
		{
			for (int i = 0; i < getMultiplexCount(); i++) process(false, i); //process all if value updated from a non-iterative input
		}
		else
		{
			process(false, multiplexIndex);
		}
	}
}

void Mapping::inputParameterRangeChanged(MappingInput *)
{
	updateMappingChain();
}

void Mapping::onContainerParameterChangedInternal(Parameter * p)
{
	Processor::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue() && !forceDisabled && !enabled->boolValue())
		{
			if (updateRate->enabled) startThread();
			for (int i = 0; i < getMultiplexCount(); i++) process(false, i);
		}
		else
		{
			stopThread(1000);
		}
	}
}

void Mapping::onControllableStateChanged(Controllable* c)
{
	Processor::onControllableStateChanged(c);
	if (c == updateRate)
	{
		if (updateRate->enabled && enabled->boolValue()) startThread();
		else stopThread(1000);
	}
}

void Mapping::filterManagerNeedsRebuild(MappingFilter* afterThisFilter)
{
	if (isClearing) return;
	updateMappingChain(afterThisFilter);
}

void Mapping::filterManagerNeedsProcess()
{
	if (isClearing) return;
	process();
}


void Mapping::clearItem()
{
	Processor::clearItem();

	fm.removeFilterManagerListener(this);
	im.removeBaseManagerListener(this); 
	im.clear();
}

void Mapping::run()
{
	uint32 millis;

	while (!threadShouldExit())
	{
		if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) continue;
		
		millis = Time::getMillisecondCounter();
		
		for (int i = 0; i < getMultiplexCount(); i++) process(false, i);

		uint32 newMillis = Time::getMillisecondCounter();

		uint32 rateMillis = 1000 / updateRate->intValue();

		uint32 millisToWait = rateMillis - jmax<uint32>(newMillis - millis, 0);
		millis = newMillis;
		
		if(millisToWait > 0) sleep(millisToWait);
	}
}

ProcessorUI * Mapping::getUI()
{
	return new MappingUI(this);
}

void Mapping::highlightLinkedInspectables(bool value)
{
	Processor::highlightLinkedInspectables(value);

	for (auto& i : im.items)
	{
		i->highlightLinkedInspectables(value);
	}

	for (auto & o : om.items)
	{
		o->highlightLinkedInspectables(value);
	}
}
