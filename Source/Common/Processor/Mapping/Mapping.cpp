/*
  ==============================================================================

	Mapping.cpp
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

Mapping::Mapping(var params, Multiplex* multiplex, bool canBeDisabled) :
	Processor("Mapping", canBeDisabled),
	MultiplexTarget(multiplex),
	Thread("Mapping"),
	im(multiplex),
	mappingParams("Parameters"),
	fm(multiplex),
	om(multiplex),
	outValuesCC("Out Values"),
	processMode(VALUE_CHANGE),
	isRebuilding(false),
	isProcessing(false),
	shouldRebuildAfterProcess(false),
	inputIsLocked(false),
	mappingNotifier(10)
{
	type = MAPPING;

	updateRate = mappingParams.addIntParameter("Update rate", "This is the update rate at which the mapping is processing. This is used only when continuous filters like Smooth and Damping are presents", 50, 1, 500, false);

	forceContinuousProcess = mappingParams.addBoolParameter("Force Continuous Process", "This will force the mapping to process continuously, even if no continuous filters are present", false);

	sendOnInputChangeOnly = mappingParams.addBoolParameter("Send On Input Change Only", "This decides whether the Mapping Outputs are always triggered when a filter changes, or only when an input has changed.", false);
	sendOnOutputChangeOnly = mappingParams.addBoolParameter("Send On Output Change Only", "This decides whether the Mapping Outputs are always triggered on source change, or only when a value's filtered output has changed.", false);
	sendOnOutputChangeOnly->forceSaveValue = true;

	sendAfterLoad = mappingParams.addBoolParameter("Send After Load", "This will force sending values once after loading", false);
	sendOnActivate = mappingParams.addBoolParameter("Send on Activate", "This will force sending values once each time the mapping is activated", false);

	//updateRate->canBeDisabledByUser = true;


	addChildControllableContainer(&im);

	addChildControllableContainer(&mappingParams);
	mappingParams.editorIsCollapsed = true;

	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);

	outValuesCC.hideInEditor = true;
	addChildControllableContainer(&outValuesCC);

	fm.addFilterManagerListener(this);
	im.addBaseManagerListener(this);

	itemColor->setDefaultValue(MAPPING_COLOR);


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

void Mapping::lockInputTo(Array<Parameter*> lockParams)
{
	inputIsLocked = lockParams.size() > 0;
	im.lockInput(lockParams);
	im.hideInEditor = inputIsLocked;
}

void Mapping::checkFiltersNeedContinuousProcess()
{
	bool need = false;
	if (processMode == TIMER || forceContinuousProcess->boolValue()) need = true;

	if (!need)
	{
		for (auto& f : fm.items)
		{
			if (!f->enabled->boolValue()) continue;

			if (f->processOnSameValue)
			{
				need = true;
				break;
			}
		}
	}

	updateRate->setEnabled(need);
	if (!sendOnOutputChangeOnly->isOverriden && !isCurrentlyLoadingData) sendOnOutputChangeOnly->setDefaultValue(updateRate->enabled);
}

void Mapping::updateMappingChain(MappingFilter* afterThisFilter, bool processAfter, bool rangeOnly, bool afterProcessSendOutput)
{
	if (isCurrentlyLoadingData || isClearing) return;
	if (isRebuilding)
	{
		if (afterThisFilter == nullptr && !rangeOnly) rebuildPending = true; //avoid infinite rebuilding
		return;
	}

	if (isProcessing && !rangeOnly)
	{
		shouldRebuildAfterProcess = true;
		return;
	}

	{
		//enter in scope for lock
		GenericScopedLock lock(mappingLock);
		isRebuilding = true;
		rebuildPending = false;

		if (!rangeOnly) outValuesCC.clear();

		for (int i = 0; i < getMultiplexCount(); i++)
		{
			if (afterThisFilter == nullptr) fm.setupSources(im.getInputReferences(i), i); //do the whole rebuild
			else fm.rebuildFilterChain(afterThisFilter, i, rangeOnly); //only ask to rebuild after the changed filter

			Array<Parameter*> processedParams = fm.getLastFilteredParameters(i);

			ControllableContainer* outCC = &outValuesCC;

			if (!rangeOnly)
			{
				if (isMultiplexed())
				{
					ControllableContainer* multiplexOutCC = new ControllableContainer("Index " + String(i + 1));
					outValuesCC.addChildControllableContainer(multiplexOutCC, true);
					outCC = multiplexOutCC;
				}

				Array<Parameter*> mOutParams;
				for (auto& sp : processedParams)
				{
					if (sp == nullptr) continue;

					Parameter* p = ControllableFactory::createParameterFrom(sp, false, false);
					mOutParams.add(p);
					outCC->addParameter(p);
					p->setControllableFeedbackOnly(true);
					p->setNiceName("Out " + String(mOutParams.size()));
					p->setValue(sp->value);
				}

				om.setOutParams(mOutParams, i);
			}
			else
			{
				if (isMultiplexed()) outCC = outValuesCC.controllableContainers[i];

				for (int j = 0; j < processedParams.size(); j++)
				{
					if (Parameter* p = (Parameter*)outCC->controllables[j])
					{
						if (p->hasRange()) p->setRange(processedParams[j]->minimumValue, processedParams[j]->maximumValue);
					}
				}
			}
		}

		if (!rangeOnly)
		{
			mappingNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this));
			checkFiltersNeedContinuousProcess();
		}

		isRebuilding = false;
	}

	if (rebuildPending)
	{
		updateMappingChain(nullptr, processAfter, rangeOnly, afterProcessSendOutput);
	}

	if (processAfter) process(afterProcessSendOutput);
}



void Mapping::multiplexCountChanged()
{
	updateMappingChain();
}

void Mapping::multiplexPreviewIndexChanged()
{
	mappingNotifier.addMessage(new MappingEvent(MappingEvent::OUTPUT_TYPE_CHANGED, this)); //force updating output to update the good index
}

void Mapping::process(bool sendOutput, int multiplexIndex)
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (im.items.size() == 0) return;
	if (isCurrentlyLoadingData || isRebuilding || isProcessing || isClearing) return;

	if (multiplexIndex == -1) // -1 makes process all
	{
		for (int i = 0; i < getMultiplexCount(); i++) process(sendOutput, i);
		return;
	}

	//DBG("[PROCESS] Enter lock");
	{
		GenericScopedLock lock(mappingLock);
		ScopedLock filterLock(fm.filterLock);

		isProcessing = true;

		Array<Parameter*> inputs = im.getInputReferences(multiplexIndex);
		MappingFilter::ProcessResult filterResult = fm.processFilters(inputs, multiplexIndex);

		if (filterResult == MappingFilter::CHANGED || (filterResult == MappingFilter::UNCHANGED && !sendOnOutputChangeOnly->boolValue()))
		{
			Array<Parameter*> filteredParameters = fm.getLastFilteredParameters(multiplexIndex);

			ControllableContainer* outCC = isMultiplexed() ? outValuesCC.controllableContainers[multiplexIndex].get() : &outValuesCC;
			for (int i = 0; i < filteredParameters.size(); i++)
			{
				if (Parameter* fp = filteredParameters[i])
				{
					if (Parameter* p = (Parameter*)outCC->controllables[i])
					{
						if (p->type == Parameter::ENUM) ((EnumParameter*)p)->setValueWithKey(((EnumParameter*)fp)->getValueKey());
						else p->setValue(fp->value);
					}
				}
			}

			if (sendOutput) om.updateOutputValues(multiplexIndex, sendOnOutputChangeOnly->boolValue());
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

void Mapping::updateContinuousProcess()
{
	if ((!canBeDisabled || enabled->boolValue()) && !forceDisabled)
	{
		if (updateRate->enabled) startThread();
		//for (int i = 0; i < getMultiplexCount(); i++) process(false, i);
	}
	else
	{
		stopThread(1000);
	}
}

void Mapping::setForceDisabled(bool value, bool force)
{
	bool changed = value != forceDisabled;

	Processor::setForceDisabled(value, force);

	if (changed || force)
	{
		if (!forceDisabled && sendOnActivate->boolValue())
		{
			process();
			//for (int i = 0; i < getMultiplexCount(); i++) om.updateOutputValues(i);
		}
	}

	updateContinuousProcess();
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
	if (!inputIsLocked) im.loadJSONData(data.getProperty("im", var()));
	mappingParams.loadJSONData(data.getProperty("params", var()));
	fm.loadJSONData(data.getProperty("filters", var()));
	om.loadJSONData(data.getProperty("outputs", var()));

	updateMappingChain();
}

void Mapping::afterLoadJSONDataInternal()
{
	//force here sendOutputOnchangeOnly overriden to avoid auto set after load, i'm sure there is a better way to do this
	
	bool prevOverriden = sendOnOutputChangeOnly->isOverriden;
	sendOnOutputChangeOnly->isOverriden = true;
	updateMappingChain(nullptr, false);
	sendOnOutputChangeOnly->isOverriden = prevOverriden;

	if (sendAfterLoad->boolValue())
	{
		if ((enabled != nullptr && enabled->boolValue()) && !forceDisabled)
		{
			for (int i = 0; i < getMultiplexCount(); i++) om.updateOutputValues(i);
		}
	}
	updateContinuousProcess();
}

void Mapping::itemAdded(MappingInput* item)
{
	item->addMappingInputListener(this);
	if (item->inputReference != nullptr) updateMappingChain();
}

void Mapping::itemsAdded(Array<MappingInput*> items)
{
	for (auto& item : items)
	{
		item->addMappingInputListener(this);
	}
	updateMappingChain();
}

void Mapping::itemRemoved(MappingInput* item)
{
	item->removeMappingInputListener(this);
	updateMappingChain();
}

void Mapping::itemsRemoved(Array<MappingInput*> items)
{
	for (auto& item : items)
	{
		item->removeMappingInputListener(this);
	}
	updateMappingChain();
}


void Mapping::itemsReordered()
{
	updateMappingChain();
}

void Mapping::inputReferenceChanged(MappingInput*)
{
	if (Engine::mainEngine->isClearing) return;

	updateMappingChain();
}

void Mapping::inputParameterValueChanged(MappingInput* mi, int multiplexIndex)
{
	if (!mi->triggersProcess->boolValue()) return;

	if (processMode == VALUE_CHANGE && !isThreadRunning())
	{
		process(true, multiplexIndex);
	}
}

void Mapping::inputParameterRangeChanged(MappingInput*)
{
	updateMappingChain(nullptr);
}

void Mapping::onContainerParameterChangedInternal(Parameter* p)
{
	Processor::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		if (enabled->boolValue() && !forceDisabled)
		{
			process();
			//for (int i = 0; i < getMultiplexCount(); i++) om.updateOutputValues(i);
		}
		updateContinuousProcess();
	}
}

void Mapping::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Processor::onControllableFeedbackUpdateInternal(cc, c);
	if (c == forceContinuousProcess)
	{
		checkFiltersNeedContinuousProcess();
		updateContinuousProcess();
	}
}

void Mapping::onControllableStateChanged(Controllable* c)
{
	Processor::onControllableStateChanged(c);
	if (c == updateRate)
	{
		if (updateRate->enabled && !forceDisabled && (!canBeDisabled || enabled->boolValue())) startThread();
		else stopThread(1000);
	}
}

void Mapping::filterManagerNeedsRebuild(MappingFilter* afterThisFilter, bool rangeOnly)
{
	if (isClearing) return;
	if (isMultiplexed() && multiplex->isChangingCount) return; //this is to avoid rebuilding for all filters that trigger a rebuild on multiplex count change
	updateMappingChain(afterThisFilter, true, rangeOnly, !sendOnInputChangeOnly->boolValue());
}

void Mapping::filterManagerNeedsProcess()
{
	if (isClearing) return;
	process(!sendOnInputChangeOnly->boolValue());
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
	wait(50); //make sure direct calls have been done before running this (especially if it was loading)

	double millis;

	while (!threadShouldExit())
	{
		double rateMillis = 1000.0 / updateRate->intValue();

		if ((canBeDisabled && !enabled->boolValue()) || forceDisabled)
		{
			wait(rateMillis);
			continue;
		}

		millis = Time::getMillisecondCounterHiRes();

		process();

		double newMillis = Time::getMillisecondCounterHiRes();

		double millisToWait = rateMillis - jlimit<double>(0, rateMillis, newMillis - millis);
		millis = newMillis;

		if (millisToWait > 0) wait(millisToWait);
	}
}

ProcessorUI* Mapping::getUI()
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

	for (auto& o : om.items)
	{
		o->highlightLinkedInspectables(value);
	}
}
