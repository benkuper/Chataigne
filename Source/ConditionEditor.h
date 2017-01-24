/*
  ==============================================================================

    ConditionEditor.h
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ConditionEditor_H_INCLUDED
#define ConditionEditor_H_INCLUDED

#include "BaseItemEditor.h"
#include "Condition.h"
#include "TargetParameterUI.h"
#include "BaseComparatorUI.h"

class ConditionEditor :
	public BaseItemEditor,
	public Condition::ConditionListener
{
public:
	ConditionEditor(Condition *, bool isRoot);
	virtual ~ConditionEditor();

	
	void resizedInternalContent(Rectangle<int> &r) override;

	Condition * condition;
	ScopedPointer<TargetParameterUI> targetUI;
	ScopedPointer<ControllableUI> sourceFeedbackUI;
	ScopedPointer<BaseComparatorUI> comparatorUI;
	
	void paintOverChildren(Graphics &g) override;
	void updateSourceUI();

	void conditionSourceChanged(Condition *) override;
	void conditionValidationChanged(Condition *) override;

	void childBoundsChanged(Component *) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionEditor)
};




#endif  // ConditionEditor_H_INCLUDED
