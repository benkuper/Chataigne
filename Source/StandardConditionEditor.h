/*
  ==============================================================================

    StandardConditionEditor.h
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef StandardConditionEditor_H_INCLUDED
#define StandardConditionEditor_H_INCLUDED

#include "ConditionEditor.h"
#include "StandardCondition.h"
#include "BaseComparatorUI.h"

class StandardConditionEditor :
	public ConditionEditor
{
public:
	StandardConditionEditor(StandardCondition *, bool isRoot);
	virtual ~StandardConditionEditor();

	StandardCondition * standardCondition;
	ScopedPointer<TargetParameterUI> targetUI;
	ScopedPointer<ControllableUI> sourceFeedbackUI;
	ScopedPointer<BaseComparatorUI> comparatorUI;
	
	void setCollapsed(bool value, bool force = false, bool animate = true) override;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	void updateUI() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StandardConditionEditor)
};




#endif  // StandardConditionEditor_H_INCLUDED
