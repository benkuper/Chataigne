/*
  ==============================================================================

    TimeCueManager.h
    Created: 6 Apr 2017 11:13:27am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECUEMANAGER_H_INCLUDED
#define TIMECUEMANAGER_H_INCLUDED


#include "TimeCue.h"

class TimeCueComparator
{
public:
	int compareElements(TimeCue * t1, TimeCue * t2)
	{
		if (t1->time->floatValue() < t2->time->floatValue()) return -1;
		else if (t1->time->floatValue() > t2->time->floatValue()) return 1;
		return 0;
	}
};

class TimeCueManager :
	public BaseManager<TimeCue>
{
public:
	TimeCueManager();
	~TimeCueManager();

	static TimeCueComparator comparator;

	void addCueAt(float time);
	void reorderItems() override;

	Array<float> getAllCueTimes(float minTime = 0, float maxTime = 0);
	float getNearestCueForTime(float time);
	
	float getNextCueForTime(float time);
	float getPrevCueForTime(float time, float goToPreviousThreshold);

    void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
};


#endif  // TIMECUEMANAGER_H_INCLUDED
