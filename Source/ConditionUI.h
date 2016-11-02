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
#include "TargetParameterUI.h"
#include "BaseComparatorUI.h"

class ConditionUI :
	public BaseItemUI<Condition>,
	public Condition::ConditionListener
{
public:
	ConditionUI(Condition *);
	virtual ~ConditionUI();

	void resized() override;

	Condition * condition;
	ScopedPointer<TargetParameterUI> targetUI;
	ScopedPointer<ControllableUI> sourceFeedbackUI;
	ScopedPointer<BaseComparatorUI> comparatorUI;
	
	void paintOverChildren(Graphics &g) override;
	void updateSourceUI();

	void conditionSourceChanged(Condition *);
	void conditionValidationChanged(Condition *);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionUI)
};




#endif  // CONDITIONUI_H_INCLUDED
