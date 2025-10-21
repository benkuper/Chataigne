/*
  ==============================================================================

	Mapping.h
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

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
	BoolParameter* forceContinuousProcess;
	BoolParameter* sendOnInputChangeOnly;
	BoolParameter* sendOnOutputChangeOnly;
	BoolParameter* sendAfterLoad;
	BoolParameter* sendOnActivate;

	enum ProcessMode { VALUE_CHANGE, MANUAL, TIMER };
	ProcessMode processMode;

	CriticalSection mappingLock;
	bool isRebuilding;
	bool isProcessing;
	bool shouldRebuildAfterProcess;
	bool rebuildPending; //force rebuilding if a rebuild has been called while already rebuilding

	void setProcessMode(ProcessMode mode);

	bool inputIsLocked;
	void lockInputTo(Array<Parameter*> lockParam);
	void checkFiltersNeedContinuousProcess();

	void updateMappingChain(MappingFilter * afterThisFilter = nullptr, bool processAfter = true, bool rangeOnly = false, bool afterProcessSendOutput = true, int multiplexIndex = -1); //will host warnings and type change checks
	virtual void multiplexCountChanged() override;
	virtual void multiplexPreviewIndexChanged() override;

	void process(bool sendOutput = true, int multiplexIndex = -1, bool forceSend = false);

	void updateContinuousProcess();

	void setForceDisabled(bool value, bool force = false, bool fromActivation = false) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	void itemAdded(MappingInput*) override;
	void itemsAdded(Array<MappingInput*>) override;
	void itemRemoved(MappingInput*) override;
	void itemsRemoved(Array<MappingInput*>) override;
	void itemsReordered() override; //MappingInput

	void inputReferenceChanged(MappingInput*, int multiplexIndex) override;
	void inputParameterValueChanged(MappingInput*, int multiplexIndex) override;
	void inputParameterRangeChanged(MappingInput*) override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void onControllableStateChanged(Controllable* c) override;

	void filterManagerNeedsRebuild(MappingFilter* afterThisFilter, bool rangeOnly) override;
	void filterManagerNeedsProcess() override;

	virtual void clearItem() override;
	virtual void run() override;
	virtual void highlightLinkedInspectables(bool value) override;

	ProcessorUI* getUI() override;

	DECLARE_ASYNC_EVENT(Mapping, Mapping, mapping, { OUTPUT_TYPE_CHANGED }, EVENT_ITEM_CHECK )

	DECLARE_TYPE("Mapping")
};