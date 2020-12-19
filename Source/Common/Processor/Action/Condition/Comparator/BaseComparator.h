/*
  ==============================================================================

    BaseComparator.h
    Created: 2 Nov 2016 8:54:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BaseComparatorUI;

class BaseComparator :
	public ControllableContainer
{
public:
	BaseComparator(Array<WeakReference<Controllable>> sources);

	virtual ~BaseComparator();

	Array<bool> isValids;
	Array<bool> rawIsValids; //for toggle behaviour

	void setValid(int iterationIndex, bool value);

	void forceToggleState(bool value);

	Array<WeakReference<Controllable>> sources;
	Controllable * reference;
	EnumParameter * compareFunction;
	BoolParameter * alwaysTrigger;
	BoolParameter* toggleMode;

	Identifier currentFunctionId;

	void setSources(Array<WeakReference<Controllable>> newSources);

	void addCompareOption(const String &name, const Identifier &func);

	virtual void compare(int iterationIndex) = 0; // to override

	virtual void forceCheck();

	void onContainerParameterChanged(Parameter *) override;
	void parameterRangeChanged(Parameter * p) override;

	class ComparatorListener
	{
	public:
		virtual ~ComparatorListener() {}
		virtual void comparatorValidationChanged(BaseComparator *, int iterationIndex) {}
	};

	ListenerList<ComparatorListener> comparatorListeners;
	void addComparatorListener(ComparatorListener* newListener) { comparatorListeners.add(newListener); }
	void removeComparatorListener(ComparatorListener* listener) { comparatorListeners.remove(listener); }

	virtual BaseComparatorUI * createUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseComparator)

	WeakReference<BaseComparator>::Master masterReference;
};

class TriggerComparator :
	public BaseComparator
{
public:
	TriggerComparator(Array<WeakReference<Controllable>> sources);
	virtual ~TriggerComparator();

	const Identifier triggeredId = "triggered";

	Array<WeakReference<Trigger>> sourceTriggers;

	virtual void compare(int iterationIndex) override;

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