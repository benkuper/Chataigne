/*
  ==============================================================================

    StandardConditionEditor.cpp
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "StandardConditionEditor.h"

StandardConditionEditor::StandardConditionEditor(StandardCondition * _condition, bool isRoot) :
	ConditionEditor(_condition, isRoot),
	standardCondition(_condition)
{
	targetUI = new ModuleInputValueChooserUI(standardCondition->sourceTarget, false);

	addAndMakeVisible(targetUI);
	setSize(100, 50);
	updateUI();
}

StandardConditionEditor::~StandardConditionEditor()
{
}

void StandardConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	ConditionEditor::resizedInternalContent(r);
	r.setY(r.getBottom());

	Rectangle<int> sr = r.withHeight(16).reduced(2,0);
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(sr.removeFromRight(jmax<int>(sr.getWidth() / 3, 60)));
		sr.removeFromRight(2);
	}
	targetUI->setBounds(sr);

	r.translate(0, 18);

	if (comparatorUI != nullptr)
	{
		r.setHeight(comparatorUI->getHeight());
		comparatorUI->setBounds(r);
	} 
	r.translate(0, 2);
	
}

void StandardConditionEditor::updateUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI);
	if (standardCondition->sourceControllable != nullptr)
	{
		sourceFeedbackUI = standardCondition->sourceControllable->createDefaultUI();
		sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI);
	}

	if (comparatorUI != nullptr)
	{
		removeChildComponent(comparatorUI);
		comparatorUI = nullptr;
	}
	if (standardCondition->comparator != nullptr)
	{
		comparatorUI = standardCondition->comparator->createUI();
		addAndMakeVisible(comparatorUI);
	}

	resized();
}