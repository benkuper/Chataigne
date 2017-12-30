/*
  ==============================================================================

    MetronomeConditionEditor.h
    Created: 19 Nov 2017 11:44:01pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MetronomeCondition.h"
#include "ConditionEditor.h"

class MetronomeConditionEditor :
	public ConditionEditor
{
public:
	MetronomeConditionEditor(MetronomeCondition * metronomeCondition, bool isRoot);
	virtual ~MetronomeConditionEditor();

	MetronomeCondition * metronomeCondition;
	ScopedPointer<FloatSliderUI> frequencyUI;
	ScopedPointer<FloatSliderUI> onTimeUI;;
	//ScopedPointer<BoolToggleUI> runningUI;

	void resizedInternalContent(Rectangle<int> &r) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MetronomeConditionEditor)
};

