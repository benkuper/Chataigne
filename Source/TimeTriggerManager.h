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
	public BaseManager<TimeTrigger>
{
public:
	TimeTriggerManager();
	~TimeTriggerManager();

	static TimeTriggerComparator comparator;

	void addItemInternal(TimeTrigger *, var data) override;
	void removeItemInternal(TimeTrigger *) override;
	
	void reorderTriggers();

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeTriggerManager)
};



#endif  // TIMETRIGGERMANAGER_H_INCLUDED
