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
	processOnSameValue(false),
	autoSetRange(true),
	filterAsyncNotifier(10)
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

bool MappingFilter::setupSources(Array<Parameter*> sources)
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

	sourceParams.clear();
	previousValues = var();
	sourceParams = Array<WeakReference<Parameter>>(sources.getRawDataPointer(), sources.size());

	for (auto& source : sourceParams)
	{
		source->addParameterListener(this);
	}

	setupParametersInternal();

	for (auto& filteredParameter : filteredParameters)
	{
		filteredParameter->isControllableFeedbackOnly = true;
		filteredParameter->addParameterListener(this);
	}

	filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));

	return true;
}

void MappingFilter::setupParametersInternal()
{
	filteredParameters.clear();
	for (auto& source : sourceParams)
	{
		Parameter* p = setupSingleParameterInternal(source);
		filteredParameters.add(p);
	}
}

Parameter* MappingFilter::setupSingleParameterInternal(Parameter* source)
{
	Parameter* p = ControllableFactory::createParameterFrom(source, true, true);
	p->isSavable = false;
	p->setControllableFeedbackOnly(true);
	return p;
}

void MappingFilter::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == enabled) mappingFilterListeners.call(&FilterListener::filterStateChanged, this);
}

void MappingFilter::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* p)
{
	if (cc == &filterParams)
	{
		filterParamChanged((Parameter*)p);
		mappingFilterListeners.call(&FilterListener::filterNeedsProcess, this);
	}
}

bool MappingFilter::process()
{
	if (!enabled->boolValue() || isClearing) return false; //default or disabled does nothing
	if (!processOnSameValue)
	{
		if (sourceParams.size() == previousValues.size())
		{
			bool hasChanged = false;
			for (int i = 0; i < sourceParams.size(); i++)
			{
				if (sourceParams[i].wasObjectDeleted()) break;

				hasChanged |= !sourceParams[i]->checkValueIsTheSame(sourceParams[i]->getValue(), previousValues[i]);
				previousValues[i] = sourceParams[i]->getValue().clone();
			}

			if (!hasChanged) return false;
		}
		else
		{
			previousValues = var();
			for (int i = 0; i < sourceParams.size(); i++) previousValues.append(sourceParams[i]->getValue().clone());
		}
		
	}

	return processInternal();  //avoid cross-thread crash
}

bool MappingFilter::processInternal()
{
	bool hasChanged = false;
	for (int i = 0; i < sourceParams.size(); ++i)
	{
		if (sourceParams[i].wasObjectDeleted() || filteredParameters[i] == nullptr) continue;

		if (!filterTypeFilters.isEmpty() && !filterTypeFilters.contains(sourceParams[i]->type))
		{
			filteredParameters[i]->setValue(sourceParams[i]->getValue()); //direct transfer if not supposed to be taken
			continue;
		}

		if (autoSetRange && filteredParameters.size() == sourceParams.size() && (filteredParameters[i]->minimumValue != sourceParams[i]->minimumValue
			|| filteredParameters[i]->maximumValue != sourceParams[i]->maximumValue))
		{
			filteredParameters[i]->setRange(sourceParams[i]->minimumValue, sourceParams[i]->maximumValue);
		}

		hasChanged |= processSingleParameterInternal(sourceParams[i], filteredParameters[i]);
	}

	return hasChanged;
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

void MappingFilter::parameterRangeChanged(Parameter* p)
{
	int pIndex = sourceParams.indexOf(p);

	bool changed = false;

	if (pIndex != -1 && filteredParameters.size() > pIndex)
	{
		if (Parameter* filteredParameter = filteredParameters[pIndex])
		{
			if (autoSetRange
				&& (filteredParameter->minimumValue != p->minimumValue || filteredParameter->maximumValue != p->maximumValue)
				&& filteredParameter->type == p->type)
			{
				filteredParameter->setRange(p->minimumValue, p->maximumValue);
				changed = true;
			}
		}
	}

	if (changed)
	{
		mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
	}
}
