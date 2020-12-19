/*
  ==============================================================================

    Condition.h
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Comparator/BaseComparator.h"
#include "conditions/IterativeCondition/IterativeCondition.h"

class Condition :
	public BaseItem,
	public BaseComparator::ComparatorListener
{
public:
	Condition(const String &name = "Condition", var params = var());
	virtual ~Condition();

	bool forceDisabled;
	Array<BoolParameter *> isValids; //this could be simplified for non-iterative condition

	int iterationCount;

	void setIterationCount(int count);

	virtual void onContainerParameterChangedInternal(Parameter *) override;
	virtual void setForceDisabled(bool value, bool force = false);
	virtual void forceCheck() {}

	class ConditionListener
	{
	public:
		virtual ~ConditionListener() {}
		virtual void conditionValidationChanged(Condition *, const IterativeContext &context) {}
		virtual void conditionSourceChanged(Condition *) {}
	};

	ListenerList<ConditionListener> conditionListeners;
	void addConditionListener(ConditionListener* newListener) { conditionListeners.add(newListener); }
	void removeConditionListener(ConditionListener* listener) { conditionListeners.remove(listener); }


	class ConditionEvent {
	public:
		enum Type { VALIDATION_CHANGED, SOURCE_CHANGED };
		ConditionEvent(Type type, Condition* c, const IterativeContext& context = { -1 }) : type(type), condition(c), context(context) {}
		Type type;
		Condition * condition;
		IterativeContext context;
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