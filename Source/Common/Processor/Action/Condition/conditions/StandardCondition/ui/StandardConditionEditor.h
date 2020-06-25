/*
  ==============================================================================

    StandardConditionEditor.h
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../../ui/ConditionEditor.h"
#include "../StandardCondition.h"
#include "../../../Comparator/ui/BaseComparatorUI.h"

class StandardConditionEditor :
	public ConditionEditor
{
public:
	StandardConditionEditor(StandardCondition *, bool isRoot);
	virtual ~StandardConditionEditor();

	StandardCondition * standardCondition;
	std::unique_ptr<InspectableEditor> targetUI;
	std::unique_ptr<ControllableUI> sourceFeedbackUI;
	std::unique_ptr<BaseComparatorUI> comparatorUI;
	
	void setCollapsed(bool value, bool force = false, bool animate = true, bool doNotRebuild = false) override;

	void resetAndBuild() override;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	void updateUI() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StandardConditionEditor)
};