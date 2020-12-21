/*
  ==============================================================================

    Action.h
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Processor.h"
#include "Condition/ConditionManager.h"
#include "Consequence/ConsequenceManager.h"
#include "../Iterator/Iterator.h"

class Action :
	public Processor,
	public IterativeTarget,
	public ConditionManager::ConditionManagerListener,
	public ConditionManager::ManagerListener,
	public Condition::ConditionListener,
	public EngineListener
{
public:
	Action(var params = var(), IteratorProcessor * it = nullptr);
	virtual ~Action();

	enum Role {ACTIVATE, DEACTIVATE};
	Array<Role> actionRoles;

	bool autoTriggerWhenAllConditionAreActives; //default true, but if false, let use Actions as user check tool without auto behavior (like TimeTriggers)
	
	bool forceNoOffConsequences; // independent of roles
	bool hasOffConsequences;

	ConditionManager cdm;
	std::unique_ptr<ConsequenceManager> csmOn;
	std::unique_ptr<ConsequenceManager> csmOff;

	Trigger* triggerOn; //if not iterative, there will be only one element in this

	//to allow for checking before conditions sending it, to overcome listener-order problems
	bool forceChecking;

	void updateConditionRoles();
	void setHasOffConsequences(bool value);
    virtual void updateDisables(bool force = false) override;

	void forceCheck(bool triggerIfChanged);

	virtual void triggerConsequences(bool triggerTrue, int iterationIndex = 0);

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;
	void endLoadFile() override;

	void onContainerTriggerTriggered(Trigger* t) override;
	void onContainerParameterChangedInternal(Parameter * p) override;
	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void conditionManagerValidationChanged(ConditionManager *, int iterationIndex) override;

	void itemAdded(Condition *) override;
	void itemRemoved(Condition *) override;

	virtual void highlightLinkedInspectables(bool value) override;

	ProcessorUI * getUI() override;

	class ActionListener
	{
	public:
		virtual ~ActionListener() {}
		virtual void actionEnableChanged(Action *) {}
		virtual void actionRoleChanged(Action *) {}
		virtual void actionValidationChanged(Action *) {}
	};

	ListenerList<ActionListener> actionListeners;
	void addActionListener(ActionListener* newListener) { actionListeners.add(newListener); }
	void removeActionListener(ActionListener* listener) { actionListeners.remove(listener); }


	class ActionEvent {
	public:
		enum Type { ENABLED_CHANGED, ROLE_CHANGED, VALIDATION_CHANGED };
		ActionEvent(Type type, Action * c) : type(type), action(c) {}
		Type type;
		Action * action;
	};
	QueuedNotifier<ActionEvent> actionAsyncNotifier;
	typedef QueuedNotifier<ActionEvent>::Listener AsyncListener;


	void addAsyncActionListener(AsyncListener* newListener) { actionAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedActionListener(AsyncListener* newListener) { actionAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncActionListener(AsyncListener* listener) { actionAsyncNotifier.removeListener(listener); }


	String getTypeString() const override { return "Action"; };
	//InspectableEditor * getEditor(bool /*isRoot*/) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Action)
};