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
	public MultiTimer
{
public:
	juce_DeclareSingleton(ConditionManager, true)

	ConditionManager();
	~ConditionManager();

	Factory<Condition> factory;
	Factory<Condition>::Definition* activateDef;
	Factory<Condition>::Definition* deactivateDef;
	Factory<Condition>::Definition* iterativeDef;

	enum ConditionOperator { AND, OR };
	EnumParameter* conditionOperator;

	FloatParameter* validationTime;

	Array<BoolParameter *> isValids;
	Array<FloatParameter *> validationProgresses;
	Array<bool> validationWaitings;
	Array<double> prevTimerTimes;

	bool forceDisabled;

	//iterative
	int iterationCount;

	void setIterationCount(int count);

	void setHasActivationDefinitions(bool value);

	void addItemInternal(Condition*, var data) override;
	void removeItemInternal(Condition*) override;

	void setForceDisabled(bool value, bool force = false);

	void forceCheck(); 

	void checkAllConditions(int iterationIndex, bool emptyIsValid = false, bool dispatchOnlyOnValidationChange = true);

	bool areAllConditionsValid(int iterationIndex, bool emptyIsValid = false);
	bool isAtLeastOneConditionValid(int iterationIndex, bool emptyIsValid = false);

	int getNumEnabledConditions();
	int getNumValidConditions(int iterationIndex);

	bool getIsValid(int iterationIndex, bool emptyIsValid);

	void dispatchConditionValidationChanged();

	void conditionValidationChanged(Condition*, const IterativeContext& context) override;

	void onContainerParameterChanged(Parameter*) override;

	virtual void timerCallback(int id) override;

	void afterLoadJSONDataInternal() override;

	InspectableEditor* getEditor(bool isRoot) override;

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager*, const IterativeContext &context) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }
};