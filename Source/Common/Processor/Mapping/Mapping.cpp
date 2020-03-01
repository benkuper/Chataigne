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
	outputParam(nullptr),
    processMode(VALUE_CHANGE),
    inputIsLocked(false),
    mappingAsyncNotifier(10)
{
	itemDataType = "Mapping";
	type = MAPPING;
	
	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed", false);
	continuousProcess->hideInEditor = true;

	addChildControllableContainer(&input);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);

	fm.addAsyncManagerListener(this);
	input.addMappingInputListener(this);

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

void Mapping::lockInputTo(Parameter * lockParam)
{
	inputIsLocked = lockParam != nullptr;
	input.lockInput(lockParam);
	input.hideInEditor = inputIsLocked;
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
	checkFiltersNeedContinuousProcess();
	Parameter * p = fm.getLastEnabledFilter() != nullptr ? fm.getLastEnabledFilter()->filteredParameter.get() : input.inputReference.get();

	if (outputParam == nullptr && p == nullptr) return;

	if (outputParam == nullptr || p == nullptr || outputParam->type != p->type)
	{		
		if (outputParam != nullptr) removeControllable(outputParam);
		outputParam = nullptr;

		if (p != nullptr)
		{ 
			outputParam = ControllableFactory::createParameterFrom(p, false, true);
			outputParam->setNiceName("Out value");
			outputParam->setControllableFeedbackOnly(true);
			outputParam->hideInEditor = true;
		}

		om.setOutParam(outputParam);
		mappingAsyncNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));

		if (outputParam != nullptr)
		{
			addParameter(outputParam);
		}
	} else
	{
		if(outputParam != nullptr && p != nullptr) outputParam->setRange(p->minimumValue, p->maximumValue);
	}

	process();
}

void Mapping::process(bool forceOutput)
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (input.inputReference == nullptr) return;

	Parameter * filteredParam = fm.processFilters();
	if (filteredParam == nullptr) return;

	if (outputParam == nullptr) updateMappingChain();
	if (outputParam == nullptr) return;
	outputParam->setValue(filteredParam->getValue(), false, processMode == MANUAL || forceOutput);
}

var Mapping::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("input", input.getJSONData());
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	input.loadJSONData(data.getProperty("input", var()));
	fm.loadJSONData(data.getProperty("filters", var()));
	om.loadJSONData(data.getProperty("outputs", var()));

	fm.setupSource(input.inputReference);

	updateMappingChain();
}

void Mapping::inputReferenceChanged(MappingInput *)
{
	if (Engine::mainEngine->isClearing) return;

	fm.setupSource(input.inputReference);
	updateMappingChain();
}

void Mapping::inputParameterValueChanged(MappingInput *)
{
	if(processMode == VALUE_CHANGE) process();
}

void Mapping::inputParameterRangeChanged(MappingInput *)
{
	if (fm.items.size() == 0) outputParam->setRange(input.inputReference->minimumValue, input.inputReference->maximumValue);
}

void Mapping::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == continuousProcess)
	{
		if (continuousProcess->boolValue()) startTimerHz(30);
		else stopTimer();
	} else if (p == outputParam)
	{
		om.setValue(outputParam->getValue());
	} else if (p == enabled && enabled->boolValue() && !forceDisabled)
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
	if (mf == fm.getLastEnabledFilter())
	{
		//Last item
		outputParam->setRange(mf->filteredParameter->minimumValue, mf->filteredParameter->maximumValue);
	}
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
	input.removeMappingInputListener(this); 
	input.clear();
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
	input.highlightLinkedInspectables(value);
	for (auto & o : om.items)
	{
		o->highlightLinkedInspectables(value);
	}
}
