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
	public MappingInput::Listener,
	public MappingInputManager::ManagerListener,
	public MappingFilterManager::FilterManagerListener,
	public Timer
{
public:
	Mapping(bool canBeDisabled = true);
	virtual ~Mapping();

	MappingInputManager im;
	MappingFilterManager fm;
	MappingOutputManager om;

	BoolParameter* continuousProcess;

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

	void process(bool forceOutput = false);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	void itemAdded(MappingInput*) override;
	void itemRemoved(MappingInput*) override;
	void itemsReordered() override; //MappingInput

	void inputReferenceChanged(MappingInput*) override;
	void inputParameterValueChanged(MappingInput*) override;
	void inputParameterRangeChanged(MappingInput*) override;

	void onContainerParameterChangedInternal(Parameter*) override;

	void filterManagerNeedsRebuild(MappingFilter* afterThisFilter) override;
	void filterManagerNeedsProcess() override;

	virtual void clearItem() override;
	virtual void timerCallback() override;
	virtual void highlightLinkedInspectables(bool value) override;

	ProcessorUI* getUI() override;

	class MappingEvent {
	public:
		enum Type { OUTPUT_TYPE_CHANGED };
		MappingEvent(Type type, Mapping* m) : type(type), mapping(m) {}
		Type type;
		Mapping* mapping;
	};
	QueuedNotifier<MappingEvent> mappingAsyncNotifier;
	typedef QueuedNotifier<MappingEvent>::Listener AsyncListener;

	void addAsyncMappingListener(AsyncListener* newListener) { mappingAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedMappingListener(AsyncListener* newListener) { mappingAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncMappingListener(AsyncListener* listener) { mappingAsyncNotifier.removeListener(listener); }


	static Mapping* create(var) { return new Mapping(); }
	String getTypeString() const override { return "Mapping"; };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mapping)


};