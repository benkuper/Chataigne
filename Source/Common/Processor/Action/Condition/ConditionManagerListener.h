/*
  ==============================================================================

    ConditionManagerListener.h
    Created: 5 Nov 2022 7:15:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConditionManager;

class ConditionManagerListener
{
public:
	virtual ~ConditionManagerListener() {}
	virtual void conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly) {}
};


class ConditionManagerEvent {
public:
	enum Type { VALIDATION_CHANGED, SEQUENTIAL_CONDITION_INDEX_CHANGED, MULTIPLEX_PREVIEW_CHANGED };
	ConditionManagerEvent(Type type, ConditionManager* cdm, int multiplexIndex = -1) : type(type), conditionManager(cdm), multiplexIndex(multiplexIndex) {}
	Type type;
	ConditionManager* conditionManager;
	int multiplexIndex;
};

typedef QueuedNotifier<ConditionManagerEvent>::Listener ConditionManagerAsyncListener;