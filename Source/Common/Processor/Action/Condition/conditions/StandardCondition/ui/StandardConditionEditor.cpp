/*
  ==============================================================================

	StandardConditionEditor.cpp
	Created: 28 Oct 2016 8:07:05pm
	Author:  bkupe

  ==============================================================================
*/

#include "StandardConditionEditor.h"

StandardConditionEditor::StandardConditionEditor(StandardCondition* _condition, bool isRoot) :
	ConditionEditor(_condition, isRoot),
	standardCondition(_condition)
{
	targetUI.reset(standardCondition->sourceTarget->getEditor(false));
	addChildComponent(targetUI.get());

	targetUI->setVisible(!standardCondition->editorIsCollapsed);

	setSize(100, 50);
	updateUI();
}

StandardConditionEditor::~StandardConditionEditor()
{
}

void StandardConditionEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	ConditionEditor::setCollapsed(value, force, animate, doNotRebuild);

	targetUI->setVisible(!standardCondition->editorIsCollapsed);
	if (sourceFeedbackUI != nullptr)	sourceFeedbackUI->setVisible(!standardCondition->editorIsCollapsed);
	if (comparatorUI != nullptr) comparatorUI->setVisible(!standardCondition->editorIsCollapsed);
}

void StandardConditionEditor::resizedInternalHeaderItemInternal(Rectangle<int> & r)
{
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(r.removeFromRight(jmin(r.getWidth() - 150, 250)).reduced(2));
		r.removeFromRight(2);
	}
	BaseItemEditor::resizedInternalHeaderItemInternal(r);
}

void StandardConditionEditor::resizedInternalContent(Rectangle<int> & r)
{
	ConditionEditor::resizedInternalContent(r);

	Rectangle<int> sr = r.withHeight(16).reduced(2, 0);
	targetUI->setBounds(sr);
	r.translate(0, 18);

	if (comparatorUI != nullptr)
	{
		comparatorUI->setBounds(r.withHeight(comparatorUI->getHeight()));
		r.translate(0, comparatorUI->getHeight());
	}

	r.translate(0, 2);
	r.setHeight(0);
}

void StandardConditionEditor::updateUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI.get());
	if (standardCondition->sourceControllable != nullptr)
	{
		sourceFeedbackUI.reset(standardCondition->sourceControllable->createDefaultUI());
		//sourceFeedbackUI->setForceFeedbackOnly(true);
		addChildComponent(sourceFeedbackUI.get());
		sourceFeedbackUI->setVisible(!standardCondition->editorIsCollapsed);
	}

	if (comparatorUI != nullptr)
	{
		removeChildComponent(comparatorUI.get());
		comparatorUI = nullptr;
	}
	if (standardCondition->comparator != nullptr)
	{
		comparatorUI.reset(standardCondition->comparator->createUI());
		addChildComponent(comparatorUI.get());
		comparatorUI->setVisible(!standardCondition->editorIsCollapsed);
	}

	resized();
}