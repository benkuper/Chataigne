/*
  ==============================================================================

	TimeTriggerManager.h
	Created: 10 Dec 2016 12:22:48pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "TimeTrigger.h"
#include "../../../Sequence.h"

class TriggerLayer;


class TimeTriggerManager :
	public BaseManager<TimeTrigger>,
	public Sequence::SequenceListener
{
public:
	TimeTriggerManager(TriggerLayer * layer, Sequence * sequence);
	~TimeTriggerManager();

	TriggerLayer * layer;
	Sequence * sequence;

	TimeTrigger * addTriggerAt(float time, float flagYPos);

	void addItemInternal(TimeTrigger * t, var data) override;

	Array<TimeTrigger *> addItemsFromClipboard(bool showWarning = true) override;
	bool canAddItemOfType(const String & typeToCheck) override;

	Array<TimeTrigger *> getTriggersInTimespan(float startTime, float endTime, bool includeAlreadyTriggered = false);

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void sequenceCurrentTimeChanged(Sequence * _sequence, float prevTime, bool evaluateSkippedData) override;
	void sequenceTotalTimeChanged(Sequence *) override;
	void sequenceLooped(Sequence *) override;

	static int compareTime(TimeTrigger * t1, TimeTrigger * t2);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeTriggerManager)
};