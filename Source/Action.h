/*
  ==============================================================================

    Action.h
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "BaseItem.h"
#include "ConditionManager.h"
#include "ConsequenceManager.h"

class Action :
	public BaseItem,
	public Condition::ConditionListener,
	public Consequence::ConsequenceListener
{
public:
	Action();
	virtual ~Action();

	ConditionManager cdm;
	ConsequenceManager csm;

	BoolParameter * isActive;
	BoolParameter * isValid; //whether the action can process or not

	void onContainerParameterChangedInternal(Parameter * p) override;

	class ActionListener
	{
	public:
		virtual ~ActionListener() {}
		virtual void actionEnableChanged(Action *) {}
		virtual void actionValidationChanged(Action *) {}
		virtual void actionActivationChanged(Action *) {}
	};

	ListenerList<ActionListener> actionListeners;
	void addActionListener(ActionListener* newListener) { actionListeners.add(newListener); }
	void removeActionListener(ActionListener* listener) { actionListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Action)
};





#endif  // ACTION_H_INCLUDED
