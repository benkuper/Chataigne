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
#include "Comparator/BaseComparator.h"

class Condition :
	public BaseItem,
	public BaseComparator::ComparatorListener
{
public:
	Condition(const String &name = "Condition", var params = var());
	virtual ~Condition();

	bool forceDisabled;

	BoolParameter * isValid;
	virtual void onContainerParameterChangedInternal(Parameter *) override;

	virtual void setForceDisabled(bool value, bool force = false);

	class ConditionListener
	{
	public:
		virtual ~ConditionListener() {}
		virtual void conditionValidationChanged(Condition *) {}
		virtual void conditionSourceChanged(Condition *) {}
	};

	ListenerList<ConditionListener> conditionListeners;
	void addConditionListener(ConditionListener* newListener) { conditionListeners.add(newListener); }
	void removeConditionListener(ConditionListener* listener) { conditionListeners.remove(listener); }


	class ConditionEvent {
	public:
		enum Type { VALIDATION_CHANGED, SOURCE_CHANGED };
		ConditionEvent(Type type, Condition * c) : type(type), condition(c) {}
		Type type;
		Condition * condition;
	};
	QueuedNotifier<ConditionEvent> conditionAsyncNotifier;
	typedef QueuedNotifier<ConditionEvent>::Listener AsyncListener;


	void addAsyncConditionListener(AsyncListener* newListener) { conditionAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConditionListener(AsyncListener* newListener) { conditionAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConditionListener(AsyncListener* listener) { conditionAsyncNotifier.removeListener(listener); }


	virtual String getTypeString() const override { jassert(false); return "error"; }

	virtual InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Condition)
};



#endif  // CONDITION_H_INCLUDED
