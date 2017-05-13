/*
  ==============================================================================

    Action.h
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "ConditionManager.h"
#include "ConsequenceManager.h"

class Action :
	public BaseItem,
	public ConditionManager::ConditionManagerListener
{
public:
	Action(const String &name = "Action");
	virtual ~Action();

	bool forceDisabled; //to disable from parent, ex. an ActionManager

	bool autoTriggerWhenAllConditionAreActives; //default true, but if false, let use Actions as user check tool without auto behavior (like TimeTriggers)

	ConditionManager cdm;
	ConsequenceManager csm;

	BoolParameter * isActive;
	BoolParameter * isValid; //whether the action can process or not
	Trigger * trigger;

	void setForceDisabled(bool value);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChangedInternal(Parameter * p) override;
    void onContainerTriggerTriggered(Trigger *) override;
	void conditionManagerValidationChanged(ConditionManager *) override;

	InspectableEditor * getEditor(bool isRoot) override;

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


	//InspectableEditor * getEditor(bool /*isRoot*/) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Action)
};





#endif  // ACTION_H_INCLUDED
