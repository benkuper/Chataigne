/*
  ==============================================================================

    ConditionEditor.cpp
    Created: 21 Feb 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#include "ConditionEditor.h"

ConditionEditor::ConditionEditor(Condition * _condition, bool isRoot) :
	BaseItemEditor(_condition, isRoot),
	condition(_condition)
{
	condition->addConditionListener(this);
	condition->addAsyncCoalescedValidationListener(this);
}

ConditionEditor::~ConditionEditor()
{
	if (!inspectable.wasObjectDeleted())
	{
		condition->removeConditionListener(this);
		condition->removeAsyncValidationListener(this);
	}
}

void ConditionEditor::paintOverChildren(Graphics & g)
{
	if (condition->isValid->boolValue())
	{
		g.setColour(condition->enabled->boolValue() ? GREEN_COLOR : LIGHTCONTOUR_COLOR);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	}
}

void ConditionEditor::conditionSourceChanged(Condition *)
{
	updateUI();
}

void ConditionEditor::newMessage(const Condition::ValidationAsyncEvent &)
{
	repaint();
}
