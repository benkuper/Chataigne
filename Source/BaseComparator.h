/*
  ==============================================================================

    BaseComparator.h
    Created: 2 Nov 2016 8:54:51pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASECOMPARATOR_H_INCLUDED
#define BASECOMPARATOR_H_INCLUDED

#include "ControllableContainer.h"

class BaseComparatorUI;

class BaseComparator :
	public ControllableContainer
{
public:
	BaseComparator(Controllable * source);
	virtual ~BaseComparator();

	bool isValid;
	void setValid(bool value);

	Controllable * source;
	EnumParameter * compareFunction;
	BoolParameter * alwaysDispatch;

	Identifier currentFunctionId;

	void addCompareOption(const String &name, const Identifier &func);

	virtual void compare() = 0; // to override

	void onContainerParameterChanged(Parameter *) override;

	class ComparatorListener
	{
	public:
		virtual ~ComparatorListener() {}
		virtual void comparatorValidationChanged(BaseComparator *) {}
	};

	ListenerList<ComparatorListener> comparatorListeners;
	void addComparatorListener(ComparatorListener* newListener) { comparatorListeners.add(newListener); }
	void removeComparatorListener(ComparatorListener* listener) { comparatorListeners.remove(listener); }

	virtual BaseComparatorUI * createUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseComparator)
};

class TriggerComparator :
	public BaseComparator
{
public:
	TriggerComparator(Controllable *source);
	virtual ~TriggerComparator();

	const Identifier triggeredId = "triggered";

	Trigger * sourceTrigger;

	virtual void compare() override;

	void triggerTriggered(Trigger *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerComparator)
};

class ParameterComparator :
	public BaseComparator
{
public:
	ParameterComparator(Controllable *source);
	virtual ~ParameterComparator();

	Parameter * sourceParameter;

	const Identifier valueChangeId = "valueChange";
	const Identifier always = "always";

	EnumParameter * whenToCompare;
	IntParameter * compareFrequency;

	void onExternalParameterChanged(Parameter * p) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterComparator)
};


#endif  // BASECOMPARATOR_H_INCLUDED
