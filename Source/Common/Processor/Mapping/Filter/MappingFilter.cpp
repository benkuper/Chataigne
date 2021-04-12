/*
  ==============================================================================

	MappingFilter.cpp
	Created: 28 Oct 2016 8:08:53pm
	Author:  bkupe

  ==============================================================================
*/

MappingFilter::MappingFilter(const String& name, var params, Multiplex* multiplex) :
	BaseItem(name),
	MultiplexTarget(multiplex),
	filterParams("filterParams", multiplex),
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

	if (!rangeOnly)
	{

		for (auto& source : sources) if (source == nullptr) return false; //check that all sources are valid

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

	return true;
}

void MappingFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{

	if (multiplexIndex >= 0 && filteredParameters.size() <= multiplexIndex || sourceParams.size() <= multiplexIndex) return;
	
	if (multiplexIndex == -1)
	{
		for (int i = 0; i < getMultiplexCount(); i++) setupParametersInternal(i);
		return;
	}

	if (!rangeOnly) filteredParameters[multiplexIndex]->clear();

	for (auto& source : sourceParams[multiplexIndex])
	{
		Parameter* p = setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
		if (!rangeOnly) filteredParameters[multiplexIndex]->add(p);
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
		if (p != nullptr && p->hasRange()) p->setRange(source->minimumValue, source->maximumValue);
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

		if (!filterTypeFilters.isEmpty() && !filterTypeFilters.contains(mSourceParams[i]->type))
		{
			fParam->setValue(inputs[i]->getValue()); //direct transfer if not supposed to be taken
			continue;
		}

		if (autoSetRange && filteredParameters.size() == inputs.size() && (fParam->minimumValue != inputs[i]->minimumValue
			|| fParam->maximumValue != inputs[i]->maximumValue))
		{
			fParam->setRange(inputs[i]->minimumValue, inputs[i]->maximumValue);
		}

		ProcessResult r = processSingleParameterInternal(inputs[i], fParam, multiplexIndex);

		if (r == STOP_HERE) return STOP_HERE;
		else if (r == CHANGED) result = CHANGED;
	}

	return result;
}


void MappingFilter::linkUpdated(ParamLinkContainer* c, ParameterLink* pLink)
{
	filterParamChanged(pLink->parameter);
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
	for (int i = 0; i < sourceParams.size(); i++) //iterate on all multiplexes
	{
		int pIndex = sourceParams[i].indexOf(p);

		bool changed = false;

		if (pIndex != -1 && filteredParameters[i]->size() > pIndex)
		{
			if (Parameter* filteredParameter = filteredParameters[i]->getUnchecked(pIndex))
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
			filterParamsAreDirty = true;
			mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
		}
	}
}
