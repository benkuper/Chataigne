/*
  ==============================================================================

    ConditionManager.h
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Condition.h"

class ConditionManager :
	public BaseManager<Condition>,
	public Condition::ConditionListener,
	public Timer
{
public:
	juce_DeclareSingleton(ConditionManager, true)

	ConditionManager();
	~ConditionManager();

	Factory<Condition> factory;
	Factory<Condition>::Definition* activateDef;
	Factory<Condition>::Definition* deactivateDef;

	BoolParameter* isValid;

	enum ConditionOperator { AND, OR };
	EnumParameter* conditionOperator;

	FloatParameter* validationTime;
	FloatParameter* validationProgress;

	bool validationWaiting;
	double prevTimerTime;
	bool forceDisabled;

	void setHasActivationDefinitions(bool value);

	void addItemInternal(Condition*, var data) override;
	void removeItemInternal(Condition*) override;

	void setForceDisabled(bool value, bool force = false);

	void forceLeastPriority(); //to override in child classes, to force its content to be processed in last, helping transitions to go the good way

	void checkAllConditions(bool emptyIsValid = false, bool dispatchOnlyOnValidationChange = true);

	bool areAllConditionsValid(bool emptyIsValid = false);
	bool isAtLeastOneConditionValid(bool emptyIsValid = false);

	int getNumEnabledConditions();
	int getNumValidConditions();

	bool getIsValid(bool emptyIsValid);


	void dispatchConditionValidationChanged();

	void conditionValidationChanged(Condition*) override;

	void onContainerParameterChanged(Parameter*) override;

	void loadJSONDataInternal(var data) override;

	InspectableEditor* getEditor(bool isRoot) override;

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager*) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionManager)


		// Inherited via Timer
		virtual void timerCallback() override;

};