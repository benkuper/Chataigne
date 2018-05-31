/*
  ==============================================================================

    ConditionManagerEditor.cpp
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManagerEditor.h"

ConditionManagerEditor::ConditionManagerEditor(ConditionManager * _manager, bool isRoot) :
	GenericManagerEditor<Condition>(_manager, isRoot),
	conditionManager(_manager)
{
	contourColor = conditionManager->isValid->boolValue() ? GREEN_COLOR : BG_COLOR.brighter(.3f);
	repaint();
}

ConditionManagerEditor::~ConditionManagerEditor()
{
}


void ConditionManagerEditor::itemAddedAsync(Condition *)
{
	if (manager->items.size() >= 1) resetAndBuild();
}

void ConditionManagerEditor::itemRemovedAsync(Condition *)
{
	if (manager->items.size() <= 1) resetAndBuild();
}

void ConditionManagerEditor::controllableFeedbackUpdate(Controllable * c)
{
	if (c == conditionManager->isValid)
	{
		contourColor = conditionManager->isValid->boolValue() ? GREEN_COLOR : BG_COLOR.brighter(.3f);
		repaint();
	}
}
