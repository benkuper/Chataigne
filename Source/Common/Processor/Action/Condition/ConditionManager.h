/*
  ==============================================================================

	ConditionManager.h
	Created: 28 Oct 2016 8:07:18pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConditionManager :
	public MultiplexTarget,
	public BaseManager<Condition>,
	public Condition::ConditionListener,
	public MultiTimer
{
public:
	ConditionManager(Multiplex* multiplex);
	~ConditionManager();

	Factory<Condition> factory;
	BaseFactoryDefinition<Condition>* activateDef;
	BaseFactoryDefinition<Condition>* deactivateDef;

	enum ConditionOperator { AND, OR, SEQUENTIAL };
	EnumParameter* conditionOperator;
	Array<int> sequentialConditionIndices;

	FloatParameter* validationTime;
	FloatParameter* invalidationTime;
	FloatParameter* validationProgressFeedback;

	Array<bool> isValids;
	Array<float> validationProgresses;
	Array<bool> validationTargets;
	Array<double> prevTimerTimes;

	bool forceDisabled;
	bool useValidationProgress;

	//sameSource sync check to avoid parameterListener order bug when 2 conditions have the same source but different operators
	bool isCheckingOtherConditionsWithSameSource;

	void multiplexCountChanged() override;
	void multiplexPreviewIndexChanged() override;

	bool hasActivationDefinitions();
	void setHasActivationDefinitions(bool hasActivation, bool hasDeactivation);

	void addItemInternal(Condition*, var data) override;
	void removeItemInternal(Condition*) override;

	void setForceDisabled(bool value, bool force = false);

	void setValid(int multiplexIndex, bool value, bool dispatchOnlyOnValidationChange = true);
	void setValidationProgress(int multiplexIndex, float value);

	void setSequentialConditionIndices(int index, int multiplexIndex = -1);

	void forceCheck();

	void checkAllConditions(int multiplexIndex, bool emptyIsValid = false, bool dispatchOnlyOnValidationChange = true, int sourceConditionIndex = -1);

	bool areAllConditionsValid(int multiplexIndex, bool emptyIsValid = false);
	bool isAtLeastOneConditionValid(int multiplexIndex, bool emptyIsValid = false);

	int getNumEnabledConditions();
	int getNumValidConditions(int multiplexIndex = 0);

	bool getIsValid(int multiplexIndex = 0, bool emptyIsValid = false);

	void dispatchConditionValidationChanged(int multiplexIndex, bool dispatchOnChangeOnly);

	void conditionValidationChanged(Condition*, int multiplexIndex, bool dispatchOnChangeOnly) override;

	void onContainerParameterChanged(Parameter*) override;

	virtual void timerCallback(int id) override;

	void afterLoadJSONDataInternal() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }

	QueuedNotifier<ConditionManagerEvent> conditionManagerAsyncNotifier;
	void addAsyncConditionManagerListener(ConditionManagerAsyncListener* newListener) { conditionManagerAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConditionManagerListener(ConditionManagerAsyncListener* newListener) { conditionManagerAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConditionManagerListener(ConditionManagerAsyncListener* listener) { conditionManagerAsyncNotifier.removeListener(listener); }
};