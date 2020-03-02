/*
  ==============================================================================

	MappingFilter.cpp
	Created: 28 Oct 2016 8:08:53pm
	Author:  bkupe

  ==============================================================================
*/

#include "MappingFilter.h"
#include "ui/MappingFilterEditor.h"

MappingFilter::MappingFilter(const String& name, var params) :
	BaseItem(name),
	filterParams("filterParams"),
	needsContinuousProcess(false),
	autoSetRange(true),
	mappingFilterAsyncNotifier(10)
{
	isSelectable = false;

	filterParams.hideEditorHeader = true;
	addChildControllableContainer(&filterParams);
	filterParams.addControllableContainerListener(this);
}

MappingFilter::~MappingFilter()
{
	clearItem();
}

bool MappingFilter::setupSources(Array<Parameter *> sources)
{
	if (isClearing) return false;
	for (auto& source : sources) if (source == nullptr) return false; //check that all sources are valid

	for (auto& sourceParam : sourceParams)
	{
		if (sourceParam != nullptr) sourceParam->removeParameterListener(this);
	}

	for (auto& filteredParameter : filteredParameters)
	{
		if (filteredParameter != nullptr)
		{
			filteredParameter->removeParameterListener(this);
			removeControllable(filteredParameter);
		}
	}

	filteredParameters.clear();
	sourceParams.clear();

	sourceParams = sources;
	
	for(auto & source:sourceParams)
	{
		source->addParameterListener(this);
	}

	setupParametersInternal();

	for (auto& filteredParameter : filteredParameters)
	{
		filteredParameter->isControllableFeedbackOnly = true;
		filteredParameter->addParameterListener(this);
	}

	mappingFilterListeners.call(&FilterListener::filteredParamChanged, this);
	mappingFilterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_PARAM_CHANGED, this));
}

bool MappingFilter::setupParametersInternal()
{
	for (auto& source : sourceParams)
	{
		Parameter* p = setupSingleParameterInternal(source);
		filteredParameters.add(p);
	}

	return true;
}

Parameter * MappingFilter::setupSingleParameterInternal(Parameter * source)
{
	return ControllableFactory::createParameterFrom(source, true, true);
}

void MappingFilter::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* p)
{
	if (cc == &filterParams)
	{
		filterParamChanged((Parameter*)p);
		mappingFilterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_PARAM_CHANGED, this));
	}
}

void MappingFilter::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == enabled)
	{
		mappingFilterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_STATE_CHANGED, this));
	}
}

bool MappingFilter::process()
{
	if (!enabled->boolValue() || isClearing) return false; //default or disabled does nothing

	return processInternal();  //avoid cross-thread crash
}

bool MappingFilter::processInternal()
{
	for (int i = 0; i < sourceParams.size(); i++)
	{
		bool result = processSingleParameterInternal(i);
		if (!result) return false;
	}

	return true;
}

bool MappingFilter::processSingleParameterInternal(int index)
{
	if (autoSetRange && (filteredParameters[index]->minimumValue != sourceParams[index]->minimumValue 
						|| filteredParameters[index]->maximumValue != sourceParams[index]->maximumValue))
	{
		filteredParameters[index]->setRange(sourceParams[index]->minimumValue, sourceParams[index]->maximumValue);
	}

	return true;
}

void MappingFilter::clearItem()
{
	BaseItem::clearItem();

	for (auto& sourceParam : sourceParams)
	{
		if (sourceParam != nullptr)
		{
			sourceParam->removeParameterListener(this);
		}
	}

	sourceParams.clear();
	filteredParameters.clear();
}

var MappingFilter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("filterParams", filterParams.getJSONData());
	return data;
}

void MappingFilter::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	filterParams.loadJSONData(data.getProperty("filterParams", var()));
}
InspectableEditor* MappingFilter::getEditor(bool isRoot)
{
	return new MappingFilterEditor(this, isRoot);
}

void MappingFilter::parameterRangeChanged(Parameter * p)
{
	int pIndex = sourceParams.indexOf(p);
	if (pIndex != -1 && filteredParameters.size() > pIndex)
	{
		if (Parameter* filteredParameter = filteredParameters[pIndex])
		{
			if (autoSetRange
				&& (filteredParameter->minimumValue != p->minimumValue || filteredParameter->maximumValue != p->maximumValue)
				&& filteredParameter->type == p->type)
			{
				filteredParameter->setRange(p->minimumValue, p->maximumValue);
			}
		}
	}

	mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
}
