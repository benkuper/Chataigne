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
	inputIsLocked(false),
	outputParam(nullptr),
	mappingAsyncNotifier(10)
{
	itemDataType = "Mapping";
	type = MAPPING;

	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed", false);
	continuousProcess->hideInEditor = true;

	addChildControllableContainer(&input);
	//addChildControllableContainer(&cdm);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);
	
	fm.addAsyncManagerListener(this);
	input.addMappingInputListener(this);

	helpID = "Mapping";
}

Mapping::~Mapping()
{
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
	for (auto &f : fm.items)
	{
		if (f->needsContinuousProcess)
		{
			need = true;
			break;
		}
	}

	continuousProcess->setValue(need);
}

void Mapping::updateMappingChain()
{
	checkFiltersNeedContinuousProcess();
	Parameter * p = fm.items.size() > 0 ? fm.items[fm.items.size() - 1]->filteredParameter.get() : input.inputReference;
	
	if (outputParam == nullptr && p == nullptr) return;

	if (outputParam == nullptr || p == nullptr || outputParam->type != p->type)
	{		
		if (outputParam != nullptr) removeControllable(outputParam);
		outputParam = nullptr;

		if (p != nullptr)
		{ 
			outputParam = ControllableFactory::createParameterFrom(p, false, true);
			outputParam->setNiceName("Out value");
			outputParam->isEditable = false;
			outputParam->hideInEditor = true;
		}

		om.setOutParam(outputParam);
		mappingAsyncNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));

		if (outputParam != nullptr)
		{
			addParameter(outputParam);
		}
	}
}

void Mapping::process()
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (input.inputReference == nullptr) return;
	//if (!cdm.getIsValid(true)) return;

	Parameter * filteredParam = fm.processFilters();
	if (filteredParam == nullptr) return;

	if (outputParam == nullptr) updateMappingChain();
	if (outputParam == nullptr) return;
	outputParam->setValue(filteredParam->getValue());
}

var Mapping::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("input", input.getJSONData());
	//data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	input.loadJSONData(data.getProperty("input", var()));
	//cdm.loadJSONData(data.getProperty("conditions", var()));
	fm.loadJSONData(data.getProperty("filters", var()));
	om.loadJSONData(data.getProperty("outputs", var()));

	fm.setupSource(input.inputReference);

	updateMappingChain();
}

void Mapping::inputReferenceChanged(MappingInput *)
{
	fm.setupSource(input.inputReference);
	updateMappingChain();
}

void Mapping::inputParameterValueChanged(MappingInput *)
{
	process();
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
	}
}

void Mapping::newMessage(const MappingFilterManager::ManagerEvent & e)
{
	updateMappingChain();
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
