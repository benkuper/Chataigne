/*
  ==============================================================================

	MappingFilter.cpp
	Created: 28 Oct 2016 8:08:53pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MappingFilter::MappingFilter(const String& name, var params, Multiplex* multiplex, bool hasChannelFilter) :
	BaseItem(name),
	MultiplexTarget(multiplex),
	canFilterChannels(hasChannelFilter),
	filterParams("filterParams", multiplex),
	isSettingUpSources(false),
	processOnSameValue(false),
	autoSetRange(true),
	filterParamsAreDirty(false),
	filterAsyncNotifier(10)
{
	isSelectable = false;

	filterParams.paramsCanBeLinked = isMultiplexed();
	filterParams.hideEditorHeader = true;
	addChildControllableContainer(&filterParams);
	filterParams.addControllableContainerListener(this);
	filterParams.addParamLinkContainerListener(this);
}

MappingFilter::~MappingFilter()
{
	clearItem();
}

bool MappingFilter::setupSources(Array<Parameter*> sources, int multiplexIndex, bool rangeOnly)
{
	if (isClearing) return false;

	isSettingUpSources = true;

	if (!rangeOnly)
	{

		for (auto& source : sources)
		{
			if (source == nullptr)
			{
				isSettingUpSources = false;
				return false; //check that all sources are valid
			}
		}

		sourceParams.ensureStorageAllocated(multiplexIndex + 1);

		Array<WeakReference<Parameter>> mSourceParams = sourceParams[multiplexIndex];
		for (auto& sourceParam : mSourceParams)
		{
			if (sourceParam != nullptr) sourceParam->removeParameterListener(this);
		}

		filteredParameters.ensureStorageAllocated(multiplexIndex + 1);

		if (filteredParameters[multiplexIndex] == nullptr) filteredParameters.set(multiplexIndex, new OwnedArray<Parameter>());

		for (auto& filteredParameter : *filteredParameters[multiplexIndex])
		{
			if (filteredParameter != nullptr)
			{
				filteredParameter->removeParameterListener(this);
				removeControllable(filteredParameter);
			}
		}

		sourceParams[multiplexIndex].clear();

		previousValues.clear();
		for (int i = 0; i < getMultiplexCount(); i++) previousValues.add(var());

		sourceParams.set(multiplexIndex, Array<WeakReference<Parameter>>(sources.getRawDataPointer(), sources.size()));
		mSourceParams = sourceParams[multiplexIndex];

		for (auto& source : mSourceParams)
		{
			source->addParameterListener(this);
		}
	}

	setupParametersInternal(multiplexIndex, rangeOnly);

	if (!rangeOnly)
	{
		for (auto& filteredParameter : *filteredParameters[multiplexIndex])
		{
			filteredParameter->isControllableFeedbackOnly = true;
			filteredParameter->addParameterListener(this);
		}

		filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}

	process(sources, multiplexIndex);

	isSettingUpSources = false;
	return true;
}

void MappingFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{

	if ((multiplexIndex >= 0 && filteredParameters.size() <= multiplexIndex) || sourceParams.size() <= multiplexIndex) return;

	if (multiplexIndex == -1)
	{
		for (int i = 0; i < getMultiplexCount(); i++) setupParametersInternal(i);
		return;
	}

	if (!rangeOnly) filteredParameters[multiplexIndex]->clear();

	int index = 0;
	for (auto& source : sourceParams[multiplexIndex])
	{
		//here if not eligible, force not setting up internal filters but generic mappingFilter setup
		Parameter* p = isChannelEligible(index) ? setupSingleParameterInternal(source, multiplexIndex, rangeOnly) : MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
		if (!rangeOnly) filteredParameters[multiplexIndex]->add(p);
		index++;
	}

}

Parameter* MappingFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;
	if (!rangeOnly)
	{
		p = ControllableFactory::createParameterFrom(source, true, true);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		int index = sourceParams[multiplexIndex].indexOf(source);
		if (index >= 0) p = (*filteredParameters[multiplexIndex])[index];
		if (autoSetRange && p != nullptr && p->hasRange()) p->setRange(source->minimumValue, source->maximumValue);
	}

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
		filterParamsAreDirty = true;
		filterParamChanged((Parameter*)p);
		mappingFilterListeners.call(&FilterListener::filterNeedsProcess, this);
	}
}

MappingFilter::ProcessResult MappingFilter::process(Array<Parameter*> inputs, int multiplexIndex)
{
	if (!enabled->boolValue()) return UNCHANGED; //default or disabled does nothing
	if (isClearing) return STOP_HERE;

	if (!processOnSameValue && !filterParamsAreDirty)
	{
		var mPrevValues = previousValues[multiplexIndex];

		if (inputs.size() == previousValues.size() && mPrevValues.isArray())
		{
			bool hasChanged = false;
			for (int i = 0; i < inputs.size(); i++)
			{
				//if (inputs[i].wasObjectDeleted()) break; //multiplex refactor : should put that back ?

				hasChanged |= !inputs[i]->checkValueIsTheSame(inputs[i]->getValue(), mPrevValues[i]);
				mPrevValues[i] = inputs[i]->getValue().clone();
			}

			if (!hasChanged) return UNCHANGED;
		}
		else
		{
			previousValues.set(multiplexIndex, var());
			for (int i = 0; i < inputs.size(); i++) previousValues[multiplexIndex].append(inputs[i]->getValue().clone());
		}

	}

	ProcessResult result = processInternal(inputs, multiplexIndex);  //avoid cross-thread crash
	filterParamsAreDirty = false;

	return result;
}

MappingFilter::ProcessResult  MappingFilter::processInternal(Array<Parameter*> inputs, int multiplexIndex)
{
	ProcessResult result = UNCHANGED;
	Array<WeakReference<Parameter>> mSourceParams = sourceParams[multiplexIndex];
	OwnedArray<Parameter>* mFilteredParams = filteredParameters[multiplexIndex];

	if (mFilteredParams == nullptr) return STOP_HERE;

	for (int i = 0; i < inputs.size() && i < mFilteredParams->size(); ++i)
	{
		if (/*inputs[i].wasObjectDeleted() || */ mFilteredParams->getUnchecked(i) == nullptr) continue;

		Parameter* fParam = mFilteredParams->getUnchecked(i);

		if (mSourceParams[i].wasObjectDeleted() || mSourceParams[i] == nullptr) continue;

		bool isTypeEligible = filterTypeFilters.isEmpty() || filterTypeFilters.contains(mSourceParams[i]->type);

		if (!isTypeEligible)
		{
			fParam->setValue(inputs[i]->getValue()); //direct transfer if not supposed to be taken
			continue;
		}

		bool channelEligible = isChannelEligible(i);

		if ((!channelEligible || autoSetRange)
			&& filteredParameters.size() == inputs.size()
			&& (fParam->minimumValue != inputs[i]->minimumValue || fParam->maximumValue != inputs[i]->maximumValue))
		{
			fParam->setRange(inputs[i]->minimumValue, inputs[i]->maximumValue);
		}

		if (!channelEligible)
		{
			if (fParam->checkValueIsTheSame(fParam->getValue(), inputs[i]->getValue())) result = CHANGED;
			fParam->setValue(inputs[i]->getValue()); //direct transfer if not supposed to be taken, but after setting range
			continue;
		}

		ProcessResult r = processSingleParameterInternal(inputs[i], fParam, multiplexIndex);

		if (r == STOP_HERE) return STOP_HERE;
		else if (r == CHANGED) result = CHANGED;
	}

	return result;
}


void MappingFilter::linkUpdated(ParamLinkContainer* c, ParameterLink* pLink)
{
	filterParamsAreDirty = true;
	filterParamChanged(pLink->parameter);
	mappingFilterListeners.call(&FilterListener::filterNeedsProcess, this);
}

void MappingFilter::listItemUpdated(ParamLinkContainer* c, ParameterLink* pLink, int multiplexIndex)
{
	filterParamsAreDirty = true;
	filterParamChanged(pLink->parameter);
	mappingFilterListeners.call(&FilterListener::filterNeedsProcess, this);
}

void MappingFilter::setExcludedChannels(Array<int> channels)
{
	excludedChannels = channels;
	mappingFilterListeners.call(&FilterListener::filterStateChanged, this);
}

bool MappingFilter::isChannelEligible(int index)
{
	return !canFilterChannels || excludedChannels.isEmpty() || sourceParams[0].size() <= 1 || !excludedChannels.contains(index);
}

void MappingFilter::multiplexPreviewIndexChanged()
{
	filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
}

void MappingFilter::clearItem()
{
	BaseItem::clearItem();

	for (auto& mSourceParams : sourceParams)
	{
		for (auto& sourceParam : mSourceParams)
		{
			if (sourceParam != nullptr)
			{
				sourceParam->removeParameterListener(this);
			}
		}
	}

	sourceParams.clear();
	filteredParameters.clear();
}

var MappingFilter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("filterParams", filterParams.getJSONData());
	if (!excludedChannels.isEmpty())
	{
		var eData;
		for (auto & c : excludedChannels) eData.append(c);
		data.getDynamicObject()->setProperty("excludedChannels", eData);
	}

	return data;
}

void MappingFilter::loadJSONDataInternal(var data)
{
	var eData = data.getProperty("excludedChannels", var());
	for (int i = 0; i < eData.size(); i++) excludedChannels.add((int)eData[i]);

	BaseItem::loadJSONDataInternal(data);
	filterParams.loadJSONData(data.getProperty("filterParams", var()));
}
InspectableEditor* MappingFilter::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new MappingFilterEditor(this, isRoot);
}

void MappingFilter::parameterRangeChanged(Parameter* p)
{
	for (int i = 0; i < sourceParams.size(); i++) //iterate on all multiplexes
	{
		int pIndex = sourceParams[i].indexOf(p);
		if (pIndex == -1 || pIndex >= filteredParameters[i]->size()) continue;

		if (Parameter* filteredParameter = filteredParameters[i]->getUnchecked(pIndex))
		{

			if ((!isChannelEligible(pIndex) || autoSetRange)
				&& (filteredParameter->minimumValue != p->minimumValue || filteredParameter->maximumValue != p->maximumValue)
				&& filteredParameter->type == p->type
				)
			{
				filteredParameter->setRange(p->minimumValue, p->maximumValue);

				filterParamsAreDirty = true;
				mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
			}
		}
	}
}
