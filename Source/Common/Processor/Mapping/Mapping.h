/*
  ==============================================================================

	Mapping.h
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Processor.h"
#include "Input/MappingInputManager.h"
#include "Filter/MappingFilterManager.h"
#include "Output/MappingOutputManager.h"

class Mapping :
	public Processor,
	public MultiplexTarget,
	public MappingInput::Listener,
	public MappingInputManager::ManagerListener,
	public MappingFilterManager::FilterManagerListener,
	public Thread
{
public:
	Mapping(var params = var(), Multiplex * multiplex = nullptr, bool canBeDisabled = true);
	virtual ~Mapping();

	MappingInputManager im;
	ControllableContainer mappingParams;
	MappingFilterManager fm;
	MappingOutputManager om;
	ControllableContainer outValuesCC;

	IntParameter* updateRate;

	enum ProcessMode { VALUE_CHANGE, MANUAL, TIMER };
	ProcessMode processMode;

	SpinLock mappingLock;
	bool isRebuilding;
	bool isProcessing;
	bool shouldRebuildAfterProcess;

	void setProcessMode(ProcessMode mode);

	bool inputIsLocked;
	void lockInputTo(Array<Parameter*> lockParam);
	void checkFiltersNeedContinuousProcess();

	void updateMappingChain(MappingFilter * afterThisFilter = nullptr); //will host warnings and type change checks

	void process(bool forceOutput = false, int multiplexIndex = 0);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	void itemAdded(MappingInput*) override;
	void itemRemoved(MappingInput*) override;
	void itemsReordered() override; //MappingInput

	void inputReferenceChanged(MappingInput*) override;
	void inputParameterValueChanged(MappingInput*, int multiplexIndex) override;
	void inputParameterRangeChanged(MappingInput*) override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableStateChanged(Controllable* c) override;

	void filterManagerNeedsRebuild(MappingFilter* afterThisFilter) override;
	void filterManagerNeedsProcess() override;

	virtual void clearItem() override;
	virtual void run() override;
	virtual void highlightLinkedInspectables(bool value) override;

	ProcessorUI* getUI() override;

	DECLARE_ASYNC_EVENT(Mapping, Mapping, mapping, { OUTPUT_TYPE_CHANGED } )

	String getTypeString() const override { return "Mapping"; };
};