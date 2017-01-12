/*
  ==============================================================================

    TimeTriggerManager.h
    Created: 10 Dec 2016 12:22:48pm
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMETRIGGERMANAGER_H_INCLUDED
#define TIMETRIGGERMANAGER_H_INCLUDED

#include "TimeTrigger.h"
#include "Sequence.h"

class TimeTriggerComparator
{
public:
	int compareElements(TimeTrigger * t1, TimeTrigger * t2)
	{
		if (t1->time->floatValue() < t2->time->floatValue()) return -1;
		else if (t1->time->floatValue() > t2->time->floatValue()) return 1;
		return 0;
	}
};

class TimeTriggerManager :
	public BaseManager<TimeTrigger>,
	public Sequence::SequenceListener
{
public:
	TimeTriggerManager(Sequence * sequence);
	~TimeTriggerManager();

	Sequence * sequence;

	static TimeTriggerComparator comparator;

	void addTriggerAt(float time,float flagYPos);

	void reorderItems() override;

	Array<TimeTrigger *> getTriggersInTimespan(float startTime, float endTime, bool includeAlreadyTriggered = false);

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override; 

	void sequenceCurrentTimeChanged(Sequence * _sequence, float prevTime, bool evaluateSkippedData) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeTriggerManager)
};



#endif  // TIMETRIGGERMANAGER_H_INCLUDED
