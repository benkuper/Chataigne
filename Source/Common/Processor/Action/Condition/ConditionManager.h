/*
  ==============================================================================

    ConditionManager.h
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Condition.h"
#include "../../Iterator/Iterator.h"

class ConditionManager :
	public IterativeTarget,
	public BaseManager<Condition>,
	public Condition::ConditionListener,
	public MultiTimer
{
public:
	ConditionManager(IteratorProcessor* iterator);
	~ConditionManager();

	Factory<Condition> factory;
	Factory<Condition>::Definition* activateDef;
	Factory<Condition>::Definition* deactivateDef;

	enum ConditionOperator { AND, OR };
	EnumParameter* conditionOperator;

	FloatParameter* validationTime;

	Array<bool> isValids;
	Array<float> validationProgresses;
	Array<bool> validationWaitings;
	Array<double> prevTimerTimes;

	bool forceDisabled;

	void iteratorCountChanged() override;

	void setHasActivationDefinitions(bool value);

	void addItemInternal(Condition*, var data) override;
	void removeItemInternal(Condition*) override;

	void setForceDisabled(bool value, bool force = false);

	void setValid(int iterationIndex, bool value, bool dispatchOnlyOnValidationChange = true);
	void setValidationProgress(int iterationIndex, float value);

	void forceCheck();

	void checkAllConditions(int iterationIndex, bool emptyIsValid = false, bool dispatchOnlyOnValidationChange = true);

	bool areAllConditionsValid(int iterationIndex, bool emptyIsValid = false);
	bool isAtLeastOneConditionValid(int iterationIndex, bool emptyIsValid = false);

	int getNumEnabledConditions();
	int getNumValidConditions(int iterationIndex = 0);

	bool getIsValid(int iterationIndex = 0, bool emptyIsValid = false);

	void dispatchConditionValidationChanged(int iterationIndex);

	void conditionValidationChanged(Condition*, int iterationIndex) override;

	void onContainerParameterChanged(Parameter*) override;

	virtual void timerCallback(int id) override;

	void afterLoadJSONDataInternal() override;

	InspectableEditor* getEditor(bool isRoot) override;

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager*, int iterationIndex) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }
	

	class ConditionManagerEvent {
	public:
		enum Type { VALIDATION_CHANGED };
		ConditionManagerEvent(Type type, ConditionManager* cdm, int iterationIndex = -1) : type(type), conditionManager(cdm), iterationIndex(iterationIndex) {}
		Type type;
		ConditionManager* conditionManager;
		int iterationIndex;
	};

	QueuedNotifier<ConditionManagerEvent> conditionManagerAsyncNotifier;
	typedef QueuedNotifier<ConditionManagerEvent>::Listener AsyncListener;

	void addAsyncConditionManagerListener(AsyncListener* newListener) { conditionManagerAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConditionManagerListener(AsyncListener* newListener) { conditionManagerAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConditionManagerListener(AsyncListener* listener) { conditionManagerAsyncNotifier.removeListener(listener); }
};