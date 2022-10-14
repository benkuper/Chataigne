/*
  ==============================================================================

    BaseComparator.h
    Created: 2 Nov 2016 8:54:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ParameterLink;
class BaseComparatorUI;

class BaseComparator :
	public ControllableContainer,
	public MultiplexTarget
{
public:
	BaseComparator(Multiplex * multiplex);
	virtual ~BaseComparator();

	Parameter* reference;
	std::unique_ptr<ParameterLink> refLink;

	EnumParameter* compareFunction;
	Identifier currentFunctionId;
	CriticalSection compareLock;

	const Identifier changeId = "ch";

	
	void setReferenceParam(Parameter*); //go through this to have automatic link for multiplex

	void addCompareOption(const String& name, const Identifier& func);
	void updateReferenceRange(Parameter* sourceParam);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	bool compare(Parameter* sourceParam, int multiplexIndex = 0);
	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex = 0) = 0; // to override

	virtual void onContainerParameterChanged(Parameter*) override;
	virtual void compareFunctionChanged() {}

	virtual BaseComparatorUI * createUI();

	DECLARE_ASYNC_EVENT(BaseComparator, Comparator, comparator, { REFERENCE_CHANGED })
};

/*
class TriggerComparator :
	public BaseComparator
{
public:
	TriggerComparator(Array<WeakReference<Controllable>> sources);
	virtual ~TriggerComparator();

	const Identifier triggeredId = "triggered";

	Array<WeakReference<Trigger>> sourceTriggers;

	virtual bool compare(var value, int multiplexIndex) override;

	void triggerTriggered(Trigger *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerComparator)
};

class ParameterComparator :
	public BaseComparator
{
public:
	ParameterComparator(Array<WeakReference<Controllable>> sources);
	virtual ~ParameterComparator();

	Array<WeakReference<Parameter>> sourceParameters;

	const Identifier valueChangeId = "valueChange";
	const Identifier always = "always";

	void onExternalParameterValueChanged(Parameter * p) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterComparator)
};
*/