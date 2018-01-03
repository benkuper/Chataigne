/*
  ==============================================================================

    Condition.h
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITION_H_INCLUDED
#define CONDITION_H_INCLUDED

#include "JuceHeader.h"
#include "BaseComparator.h"

class Condition :
	public BaseItem,
	public BaseComparator::ComparatorListener
{
public:
	Condition(const String &name = "Condition", var params = var());
	virtual ~Condition();

	BoolParameter * isValid;
	virtual void onContainerParameterChangedInternal(Parameter *) override;

	class ConditionListener
	{
	public:
		virtual ~ConditionListener() {}
		virtual void conditionEnableChanged(Condition *) {}
		virtual void conditionValidationChanged(Condition *) {}
		virtual void conditionSourceChanged(Condition *) {}
	};

	ListenerList<ConditionListener> conditionListeners;
	void addConditionListener(ConditionListener* newListener) { conditionListeners.add(newListener); }
	void removeConditionListener(ConditionListener* listener) { conditionListeners.remove(listener); }


	class ValidationAsyncEvent {
	public:
		ValidationAsyncEvent(bool _value) : value(_value) {}
		bool value;
	};
	QueuedNotifier<ValidationAsyncEvent> validationAsyncNotifier;
	typedef QueuedNotifier<ValidationAsyncEvent>::Listener AsyncListener;


	void addAsyncValidationListener(AsyncListener* newListener) { validationAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedValidationListener(AsyncListener* newListener) { validationAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncValidationListener(AsyncListener* listener) { validationAsyncNotifier.removeListener(listener); }


	virtual String getTypeString() const override { jassert(false); return "error"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Condition)
};



#endif  // CONDITION_H_INCLUDED
