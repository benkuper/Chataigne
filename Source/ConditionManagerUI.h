/*
  ==============================================================================

    ConditionManagerUI.h
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITIONMANAGERUI_H_INCLUDED
#define CONDITIONMANAGERUI_H_INCLUDED



#include "ConditionUI.h"
#include "BaseManagerUI.h"
#include "ConditionManager.h"

class ConditionManagerUI :
	public BaseManagerUI<ConditionManager, Condition, ConditionUI>
{
public:

	ConditionManagerUI(ConditionManager *_manager);
	~ConditionManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionManagerUI)
};





#endif  // CONDITIONMANAGERUI_H_INCLUDED
