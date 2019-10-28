/*
  ==============================================================================

    Mapping.h
    Created: 28 Oct 2016 8:06:13pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Processor.h"
#include "Input/MappingInput.h"
#include "Filter/MappingFilterManager.h"
#include "Output/MappingOutputManager.h"
#include "../Action/Condition/ConditionManager.h"

class Mapping :
	public Processor,
	public MappingInput::Listener,
	public Timer,
	public MappingFilterManager::BaseManager::AsyncListener,
	public MappingFilter::FilterListener,
	public MappingFilter::AsyncListener
{
public:
	Mapping(bool canBeDisabled = true);
	virtual ~Mapping();

	MappingInput input;
	//ConditionManager cdm;
	MappingFilterManager fm;
	MappingOutputManager om;

	BoolParameter * continuousProcess;

	Parameter * outputParam;

	enum ProcessMode { VALUE_CHANGE, MANUAL, TIMER };
	ProcessMode processMode;


	void setProcessMode(ProcessMode mode);

	bool inputIsLocked;
	void lockInputTo(Parameter * lockParam);
	void checkFiltersNeedContinuousProcess();

	void updateMappingChain(); //will host warnings and type change checks

	void process(bool forceOutput = false);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void inputReferenceChanged(MappingInput *) override;
	void inputParameterValueChanged(MappingInput *) override;
	void inputParameterRangeChanged(MappingInput *) override;

	void onContainerParameterChangedInternal(Parameter *) override;

	void newMessage(const MappingFilterManager::ManagerEvent &e) override;

	void filteredParamRangeChanged(MappingFilter * mf) override;

	void newMessage(const MappingFilter::FilterEvent &e) override;

	virtual void clearItem() override;

	virtual void timerCallback() override;

	virtual void highlightLinkedInspectables(bool value) override;

	ProcessorUI * getUI() override;

	class MappingEvent {
	public:
		enum Type { OUTPUT_TYPE_CHANGED };
		MappingEvent(Type type, Mapping * m) : type(type), mapping(m) {}
		Type type;
		Mapping * mapping;
	};
	QueuedNotifier<MappingEvent> mappingAsyncNotifier;
	typedef QueuedNotifier<MappingEvent>::Listener AsyncListener;

	void addAsyncMappingListener(AsyncListener* newListener) { mappingAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedMappingListener(AsyncListener* newListener) { mappingAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncMappingListener(AsyncListener* listener) { mappingAsyncNotifier.removeListener(listener); }


	static Mapping * create(var) { return new Mapping(); }
	String getTypeString() const override { return "Mapping"; };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mapping)

	
};