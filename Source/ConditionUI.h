/*
  ==============================================================================

    ConditionUI.h
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITIONUI_H_INCLUDED
#define CONDITIONUI_H_INCLUDED

#include "BaseItemUI.h"
#include "Condition.h"

class ConditionUI :
	public BaseItemUI<Condition>
{
public:
	ConditionUI(Condition *);
	virtual ~ConditionUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionUI)
};




#endif  // CONDITIONUI_H_INCLUDED
