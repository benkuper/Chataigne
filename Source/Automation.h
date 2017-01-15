/*
  ==============================================================================

    Automation.h
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATION_H_INCLUDED
#define AUTOMATION_H_INCLUDED

#include "AutomationKey.h"
#include "BaseManager.h"

class AutomationKeyComparator
{
public:
	int compareElements(AutomationKey * t1, AutomationKey * t2)
	{
		if (t1->position->floatValue() < t2->position->floatValue()) return -1;
		else if (t1->position->floatValue() > t2->position->floatValue()) return 1;
		return 0;
	}
};

class Automation :
	public BaseManager<AutomationKey>
{
public:
	Automation();
	virtual ~Automation();

	float positionMax;

	void setPositionMax(float val);

	float getValueForPosition(float pos);

	static AutomationKeyComparator comparator;
	
	AutomationKey * createItem() override;
	void addItem(const float position, const float value);
	void reorderItems() override;

	AutomationKey * getClosestKeyForPos(float pos, int start = -1, int end = -1);

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Automation)
};



#endif  // AUTOMATION_H_INCLUDED
