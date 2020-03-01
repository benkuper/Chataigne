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
    mappingAsyncNotifier(10)
{
	itemDataType = "Mapping";
	type = MAPPING;
	
	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed", false);
	continuousProcess->hideInEditor = true;

	addChildControllableContainer(&im);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);

	fm.addAsyncManagerListener(this);
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

void Mapping::updateMappingChain()
{
	if (isCurrentlyLoadingData) return;

	checkFiltersNeedContinuousProcess();
	Array<WeakReference<Parameter>> processedParams = fm.getLastEnabledFilter() != nullptr ? fm.getLastEnabledFilter()->filteredParameters : im.getInputReferences();

	bool hasChanged = false;
	while (outputParams.size() > processedParams.size())
	{
		removeControllable(outputParams[outputParams.size() - 1].get());
		outputParams.removeLast();
		hasChanged = true;
	}

	for (int i = 0; i < processedParams.size(); i++)
	{
		if (processedParams[i] == nullptr)
		{
			if (outputParams.size() > i)
			{
				removeControllable(outputParams[i].get());
				outputParams.set(i, nullptr);
			}
			
			hasChanged = true;
		}

		if (i >= outputParams.size())
		{
			outputParams.add(nullptr);
			hasChanged = true;
		}

		if(outputParams[i] == nullptr)
		{
			if (Parameter* outputParam = ControllableFactory::createParameterFrom(processedParams[i], false, true))
			{
				outputParam->setNiceName("Out value");
				outputParam->setControllableFeedbackOnly(true);
				outputParam->hideInEditor = true;
				outputParams.set(i, outputParam);
				addParameter(outputParam);
				hasChanged = true;
			}
		}

		outputParams[i]->setRange(processedParams[i]->minimumValue, processedParams[i]->maximumValue);
	}

	if (hasChanged)
	{
		om.setOutParams(outputParams);
		mappingAsyncNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));
	}

	process();
}

void Mapping::process(bool forceOutput)
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (im.items.size() == 0) return;
	if (isCurrentlyLoadingData) return;

	Array<WeakReference<Parameter>> filteredParams = fm.processFilters();
	if (filteredParams.size() != outputParams.size())
	{
		updateMappingChain();
		return;
	}

	//updateMappingChain(); //should not need that here ? 

	for (int i = 0; i < filteredParams.size(); i++)
	{
		outputParams[i]->setValue(filteredParams[i]->getValue(), false, processMode == MANUAL || forceOutput);
	}

	om.updateOutputValues();
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

	fm.setupSources(im.getInputReferences());

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

void Mapping::inputReferenceChanged(MappingInput *)
{
	if (Engine::mainEngine->isClearing) return;

	fm.setupSources(im.getInputReferences());
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
	}/* else if (outputParams.contains(p))
	{
		om.updateOutputValues();
	}*/
	else if (p == enabled && enabled->boolValue() && !forceDisabled)
	{
		process();
	}
}

void Mapping::newMessage(const MappingFilterManager::ManagerEvent & e)
{
	if (e.type == MappingFilterManager::ManagerEvent::ITEM_ADDED)
	{
		e.getItem()->addMappingFilterListener(this);
		e.getItem()->addAsyncFilterListener(this);
	}

	if(!isCurrentlyLoadingData) updateMappingChain();
}

void Mapping::filteredParamRangeChanged(MappingFilter * mf)
{
	updateMappingChain();
	/*
	if (mf == fm.getLastEnabledFilter())
	{
		//Last item
		outputParam->setRange(mf->filteredParameter->minimumValue, mf->filteredParameter->maximumValue);
	}
	*/
}

void Mapping::newMessage(const MappingFilter::FilterEvent & e)
{
	if (e.type == MappingFilter::FilterEvent::FILTER_PARAM_CHANGED)
	{
		process();
	}
}

void Mapping::clearItem()
{
	BaseItem::clearItem();

	fm.removeAsyncManagerListener(this);
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
