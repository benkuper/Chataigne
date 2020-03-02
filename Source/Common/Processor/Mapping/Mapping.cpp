/*
  ==============================================================================

	Mapping.cpp
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"
#include "ui/MappingUI.h"

Mapping::Mapping(bool canBeDisabled) :
	Processor("Mapping", canBeDisabled),
	processMode(VALUE_CHANGE),
	inputIsLocked(false),
	mappingAsyncNotifier(10),
	outCC("Out values"),
	isRebuilding(false)
{
	itemDataType = "Mapping";
	type = MAPPING;
	
	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed", false);
	continuousProcess->hideInEditor = true;

	addChildControllableContainer(&im);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);
	addChildControllableContainer(&outCC);

	fm.addFilterManagerListener(this);
	im.addBaseManagerListener(this);

	helpID = "Mapping";
}

Mapping::~Mapping()
{
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
			if (f->needsContinuousProcess)
			{
				need = true;
				break;
			}
		}
	}

	continuousProcess->setValue(need);
}

void Mapping::updateMappingChain(MappingFilter * afterThisFilter)
{
	if (isCurrentlyLoadingData || isClearing) return;
	if (isRebuilding) return;

	stopTimer();
	{
		//enter in scope for lock
		GenericScopedLock lock(mappingLock);
		isRebuilding = true;

		if(afterThisFilter == nullptr) fm.setupSources(im.getInputReferences()); //do the whole rebuild
		else fm.rebuildFilterChain(afterThisFilter); //only ask to rebuild after the changed filter

		Array<Parameter*> processedParams = fm.getLastFilteredParameters();
		om.setOutParams(processedParams);

		mappingAsyncNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));

		checkFiltersNeedContinuousProcess();

		isRebuilding = false;
	}


	process();
}

void Mapping::process(bool forceOutput)
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (im.items.size() == 0) return;
	if (isCurrentlyLoadingData) return;

	DBG("[PROCESS] Enter lock");
	GenericScopedLock lock(mappingLock);

	Array<Parameter *> filteredParams = fm.processFilters();
	om.updateOutputValues();

	DBG("[PROCESS] Exit lock");

}

var Mapping::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("im", im.getJSONData());
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	im.loadJSONData(data.getProperty("im", var()));
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
}

void Mapping::itemRemoved(MappingInput* item)
{
	item->removeMappingInputListener(this);
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

void Mapping::inputParameterValueChanged(MappingInput *)
{
	if(processMode == VALUE_CHANGE) process();
}

void Mapping::inputParameterRangeChanged(MappingInput *)
{
	updateMappingChain();
	//if (fm.items.size() == 0) outputParam->setRange(input.inputReference->minimumValue, input.inputReference->maximumValue);
}

void Mapping::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == continuousProcess)
	{
		if (continuousProcess->boolValue()) startTimerHz(30);
		else stopTimer();
	}
	else if (p == enabled && enabled->boolValue() && !forceDisabled)
	{
		process();
	}
}

void Mapping::filterManagerNeedsRebuild(MappingFilter* afterThisFilter)
{
	updateMappingChain(afterThisFilter);
}

void Mapping::filterManagerNeedsProcess()
{
	process();
}


void Mapping::clearItem()
{
	BaseItem::clearItem();

	fm.removeFilterManagerListener(this);
	im.removeBaseManagerListener(this); 
	im.clear();
}

ProcessorUI * Mapping::getUI()
{
	return new MappingUI(this);
}

void Mapping::timerCallback()
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	process();
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
