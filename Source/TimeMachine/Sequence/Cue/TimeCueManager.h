/*
  ==============================================================================

    TimeCueManager.h
    Created: 6 Apr 2017 11:13:27am
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "TimeCue.h"

class TimeCueComparator
{
public:
	int compareElements(TimeCue * t1, TimeCue * t2)
	{
		
	}
};

class TimeCueManager :
	public BaseManager<TimeCue>
{
public:
	TimeCueManager();
	~TimeCueManager();
	void addCueAt(float time);
	void reorderItems() override;

	Array<float> getAllCueTimes(float minTime = 0, float maxTime = 0);
	float getNearestCueForTime(float time);

	Array<TimeCue *> getCuesInTimespan(float startTime, float endTime);
	
	float getNextCueForTime(float time);
	float getPrevCueForTime(float time, float goToPreviousThreshold);

	static int compareTime(TimeCue * t1, TimeCue * t2);

    void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;
};