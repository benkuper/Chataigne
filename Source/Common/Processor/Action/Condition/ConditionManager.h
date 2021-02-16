/*
  ==============================================================================

    ConditionManager.h
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Condition.h"
#include "../../Multiplex/Multiplex.h"

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
	Factory<Condition>::Definition* activateDef;
	Factory<Condition>::Definition* deactivateDef;

	enum ConditionOperator { AND, OR, SEQUENTIAL };
	EnumParameter* conditionOperator;
	Array<int> sequentialConditionIndices;

	FloatParameter* validationTime;
	FloatParameter* validationProgressFeedback;

	Array<bool> isValids;
	Array<float> validationProgresses;
	Array<bool> validationWaitings;
	Array<double> prevTimerTimes;

	bool forceDisabled;

	//sameSource sync check to avoid parameterListener order bug when 2 conditions have the same source but different operators
	bool isCheckingOtherConditionsWithSameSource;

	void multiplexCountChanged() override;
	void multiplexPreviewIndexChanged() override;

	void setHasActivationDefinitions(bool value);

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

	InspectableEditor* getEditor(bool isRoot) override;

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }
	

	class ConditionManagerEvent {
	public:
		enum Type { VALIDATION_CHANGED, SEQUENTIAL_CONDITION_INDEX_CHANGED, MULTIPLEX_PREVIEW_CHANGED };
		ConditionManagerEvent(Type type, ConditionManager* cdm, int multiplexIndex = -1) : type(type), conditionManager(cdm), multiplexIndex(multiplexIndex) {}
		Type type;
		ConditionManager* conditionManager;
		int multiplexIndex;
	};

	QueuedNotifier<ConditionManagerEvent> conditionManagerAsyncNotifier;
	typedef QueuedNotifier<ConditionManagerEvent>::Listener AsyncListener;

	void addAsyncConditionManagerListener(AsyncListener* newListener) { conditionManagerAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConditionManagerListener(AsyncListener* newListener) { conditionManagerAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConditionManagerListener(AsyncListener* listener) { conditionManagerAsyncNotifier.removeListener(listener); }
};