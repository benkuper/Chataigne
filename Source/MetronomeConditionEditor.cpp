/*
  ==============================================================================

    MetronomeConditionEditor.cpp
    Created: 19 Nov 2017 11:44:01pm
    Author:  Ben

  ==============================================================================
*/

#include "MetronomeConditionEditor.h"

MetronomeConditionEditor::MetronomeConditionEditor(MetronomeCondition * _metronomeCondition, bool isRoot) :
	ConditionEditor(_metronomeCondition,isRoot),
	metronomeCondition(_metronomeCondition)
{
	frequencyUI = metronomeCondition->frequency->createSlider();
	onTimeUI = metronomeCondition->onTime->createSlider();
	//runningUI = metronomeCondition->running->createToggle();
	addAndMakeVisible(frequencyUI);
	addAndMakeVisible(onTimeUI);
}

MetronomeConditionEditor::~MetronomeConditionEditor()
{
}

void MetronomeConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	frequencyUI->setBounds(r.withHeight(16));
	r.translate(0, frequencyUI->getHeight() + 2);
	onTimeUI->setBounds(r.withHeight(16));
	r.translate(0, onTimeUI->getHeight() + 2);
	r.setBottom(onTimeUI->getBottom() + 2);

}
