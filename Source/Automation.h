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

	float valueMax;
	float positionMax;

	static AutomationKeyComparator comparator;

	void addItemInternal(AutomationKey *, var data) override;
	void reorderKeys();

	AutomationKey * addItem(const float position, const float value);


	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Automation)
};



#endif  // AUTOMATION_H_INCLUDED
