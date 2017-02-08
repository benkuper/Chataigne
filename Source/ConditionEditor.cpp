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
	targetUI = new ModuleInputValueChooserUI(condition->sourceTarget, false);
	condition->addConditionListener(this);
	condition->addAsyncCoalescedValidationListener(this);

	addAndMakeVisible(targetUI);
	setSize(100, 50);
	updateSourceUI();
}

ConditionEditor::~ConditionEditor()
{
	if (!inspectable.wasObjectDeleted())
	{
		condition->removeConditionListener(this);
		condition->removeAsyncValidationListener(this);
	}
}

void ConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
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

void ConditionEditor::newMessage(const Condition::ValidationAsyncEvent & e)
{
	repaint();
}

void ConditionEditor::childBoundsChanged(Component *)
{
	resized();
}
