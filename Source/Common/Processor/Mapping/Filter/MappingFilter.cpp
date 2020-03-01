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
	forceOutParameterType(""),
	needsContinuousProcess(false),
	autoSetRange(true),
	mappingFilterAsyncNotifier(10)
{
	isSelectable = false;

	filterParams.hideEditorHeader = true;
	addChildControllableContainer(&filterParams);
	filterParams.addControllableContainerListener(this);

	forceOutParameterType = params.getProperty("forceType", "");
}

MappingFilter::~MappingFilter()
{
	clearItem();
}

void MappingFilter::setupSources(Array<WeakReference<Parameter>> sources)
{
	if (isClearing) return;

	for (auto& sourceParam : sourceParams)
	{
		if (sourceParam != nullptr) sourceParam->removeParameterListener(this);
	}

	for (auto& filteredParameter : filteredParameters)
	{
		if (filteredParameter != nullptr)
		{
			filteredParameter->removeParameterListener(this);
			removeControllable(filteredParameter.get());
		}
	}

	sourceParams = sources;

	filteredParameters = setupParametersInternal(sources);
	
	for(auto & source:sourceParams)
	{
		source->addParameterListener(this);
	}

	for (auto& filteredParameter : filteredParameters)
	{
		filteredParameter->isControllableFeedbackOnly = true;
		filteredParameter->addParameterListener(this);
		addParameter(filteredParameter);
	}

	mappingFilterListeners.call(&FilterListener::filteredParamChanged, this);
	mappingFilterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_PARAM_CHANGED, this));
}

Array<WeakReference<Parameter>> MappingFilter::setupParametersInternal(Array<WeakReference<Parameter>> sources)
{
	Array<WeakReference<Parameter>> result;
	if (isClearing) return result;

	for (auto& source : sources)
	{
		if (source == nullptr || source.wasObjectDeleted()) continue;

		Parameter* p = (Parameter*)ControllableFactory::createControllable(forceOutParameterType.isNotEmpty() ? forceOutParameterType : source->getTypeString());
		p->setNiceName("Out");
		if (p->value.size() == source->value.size())
		{
			if (source->hasRange()) p->setRange(source->minimumValue, source->maximumValue);
			p->setValue(source->getValue());
		}

		p->hideInEditor = true;
		result.add(p);
	}

	return result;
}

bool MappingFilter::checkSourcesAreValid(Array<WeakReference<Parameter>> sources, bool checkIfDifferent)
{
	int numSources = sources.size();
	int numFilteredParameters = filteredParameters.size();

	if (numSources == 0 || numFilteredParameters == 0 || numSources != numFilteredParameters) return false;
	for (int i = 0; i < numSources; i++)
	{
		if (sources[i] == nullptr || sources[i].wasObjectDeleted() || filteredParameters[i] == nullptr || filteredParameters[i].wasObjectDeleted()) return false;
		if (checkIfDifferent && (sources[i].get() != filteredParameters[i].get())) return false;
	}

	return true;
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

Array<WeakReference<Parameter>> MappingFilter::process(Array<WeakReference<Parameter>> sources)
{
	if (!enabled->boolValue() || isClearing) return sources; //default or disabled does nothing
	if(checkSourcesAreValid(sources ,true)) return sources;

	int numSources = sources.size();
	for (int i = 0; i < numSources; i++)
	{
		if (autoSetRange && (filteredParameters[i]->minimumValue != sourceParams[i]->minimumValue || filteredParameters[i]->maximumValue != sourceParams[i]->maximumValue))
		{
			filteredParameters[i]->setRange(sourceParams[i]->minimumValue, sourceParams[i]->maximumValue);
		}
	}
	
	processInternal();  //avoid cross-thread crash
	return filteredParameters;
}

void MappingFilter::clearItem()
{
	BaseItem::clearItem();

	//setupSource(nullptr);

	for (auto& sourceParam : sourceParams)
	{
		if (!sourceParam.wasObjectDeleted() && sourceParam != nullptr)
		{
			sourceParam->removeParameterListener(this);
		}
	}

	sourceParams.clear();
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
