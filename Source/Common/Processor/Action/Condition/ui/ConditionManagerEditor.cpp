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
	contourColor = conditionManager->getIsValid() ? GREEN_COLOR : BG_COLOR.brighter(.3f);
	repaint();

	conditionManager->addAsyncConditionManagerListener(this);
}

ConditionManagerEditor::~ConditionManagerEditor()
{
	if(!inspectable.wasObjectDeleted()) conditionManager->removeAsyncConditionManagerListener(this);
}


void ConditionManagerEditor::itemAddedAsync(Condition *)
{
	if (manager->items.size() >= 1) resetAndBuild();
}

void ConditionManagerEditor::itemRemovedAsync(Condition *)
{
	if (manager->items.size() <= 1) resetAndBuild();
}

void ConditionManagerEditor::newMessage(const ConditionManager::ConditionManagerEvent& e)
{
	if (e.type == e.VALIDATION_CHANGED)
	{
		contourColor = conditionManager->getIsValid(e.multiplexIndex) ? GREEN_COLOR : BG_COLOR.brighter(.3f);
		repaint();
	}
}
