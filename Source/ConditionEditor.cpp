/*
  ==============================================================================

    ConditionEditor.cpp
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionEditor.h"



ConditionEditor::ConditionEditor(Condition * _condition, bool isRoot) :
	BaseItemEditor(_condition, isRoot),
	condition(_condition)
{
	targetUI = condition->sourceTarget->createTargetUI();
	condition->addConditionListener(this);
	addAndMakeVisible(targetUI);
	setSize(100, 50);
	updateSourceUI();
}

ConditionEditor::~ConditionEditor()
{
	condition->removeConditionListener(this);
}

void ConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> sr = r.withHeight(headerHeight);
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(sr.removeFromRight(jmax<int>(sr.getWidth() / 3, 60)));
		sr.removeFromRight(2);
	}
	targetUI->setBounds(sr);

	r.translate(0, headerHeight);

	if (comparatorUI != nullptr)
	{
		r.setHeight(comparatorUI->getHeight());
		comparatorUI->setBounds(r);
	} 
	
}

void ConditionEditor::paintOverChildren(Graphics & g)
{
	if (condition->isValid->boolValue())
	{
		g.setColour(condition->enabled->boolValue()?GREEN_COLOR:LIGHTCONTOUR_COLOR);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	}
}

void ConditionEditor::updateSourceUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI);
	if (condition->sourceControllable != nullptr)
	{
		sourceFeedbackUI = condition->sourceControllable->createDefaultUI();
		sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI);
	}

	if (comparatorUI != nullptr)
	{
		removeChildComponent(comparatorUI);
		comparatorUI = nullptr;
	}
	if (condition->comparator != nullptr)
	{
		comparatorUI = condition->comparator->createUI();
		addAndMakeVisible(comparatorUI);
	}

	resized();
}

void ConditionEditor::conditionSourceChanged(Condition *)
{
	updateSourceUI();
}

void ConditionEditor::conditionValidationChanged(Condition *)
{
	repaint();
}

void ConditionEditor::childBoundsChanged(Component *)
{
	resized();
}
