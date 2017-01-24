/*
  ==============================================================================

    Condition.h
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITION_H_INCLUDED
#define CONDITION_H_INCLUDED

#include "BaseItem.h"
#include "BaseComparator.h"

class Condition :
	public BaseItem,
	public BaseComparator::ComparatorListener
{
public:
	Condition();
	virtual ~Condition();

	BoolParameter * isValid;
	TargetParameter * sourceTarget;
	WeakReference<Controllable> sourceControllable;
	ScopedPointer<BaseComparator> comparator;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void setSourceControllable(WeakReference<Controllable> c);
	void onContainerParameterChangedInternal(Parameter *) override;
	void comparatorValidationChanged(BaseComparator *) override;

	InspectableEditor * getEditor(bool isRoot) override;

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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Condition)
};



#endif  // CONDITION_H_INCLUDED
