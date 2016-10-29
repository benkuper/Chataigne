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

class Condition :
	public BaseItem
{
public:
	Condition();
	virtual ~Condition();

	BoolParameter * isValid; //whether the action can process or not
	BoolParameter * isActive;
	
	class ConditionListener
	{
	public:
		virtual ~ConditionListener() {}
		virtual void conditionEnableChanged(Condition *) {}
		virtual void conditionValidationChanged(Condition *) {}
		virtual void conditionActivationChanged(Condition *) {}
	};


	ListenerList<ConditionListener> conditionListeners;
	void addConditionListener(ConditionListener* newListener) { conditionListeners.add(newListener); }
	void removeConditionListener(ConditionListener* listener) { conditionListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Condition)
};



#endif  // CONDITION_H_INCLUDED
