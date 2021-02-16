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
	contourColor = conditionManager->getIsValid(conditionManager->getPreviewIndex()) ? GREEN_COLOR : BG_COLOR.brighter(.3f);

	bool isSequential = conditionManager->conditionOperator->getValueDataAsEnum<ConditionManager::ConditionOperator>() == ConditionManager::SEQUENTIAL;
	if (isSequential) updateSequentialUI();

	conditionManager->addAsyncConditionManagerListener(this);
	repaint();
}

ConditionManagerEditor::~ConditionManagerEditor()
{
	if(!inspectable.wasObjectDeleted()) conditionManager->removeAsyncConditionManagerListener(this);
}


void ConditionManagerEditor::updateSequentialUI()
{
	bool isSequential = conditionManager->conditionOperator->getValueDataAsEnum<ConditionManager::ConditionOperator>() == ConditionManager::SEQUENTIAL;

	for (int i = 0; i < manager->items.size(); i++)
	{
		if (ConditionEditor * ce = dynamic_cast<ConditionEditor*>(getEditorForInspectable(conditionManager->items[i])))
		{
			ce->isCurrentInSequential = isSequential && conditionManager->sequentialConditionIndices[conditionManager->getPreviewIndex()] == i;
			ce->repaint();
		}
	}
}

void ConditionManagerEditor::itemAddedAsync(Condition *)
{
	if (manager->items.size() >= 1) resetAndBuild();
}

void ConditionManagerEditor::itemRemovedAsync(Condition *)
{
	if (manager->items.size() <= 1) resetAndBuild();
}

void ConditionManagerEditor::resetAndBuild()
{
	GenericManagerEditor::resetAndBuild();
	
	bool isSequential = conditionManager->conditionOperator->getValueDataAsEnum<ConditionManager::ConditionOperator>() == ConditionManager::SEQUENTIAL;
	if(isSequential) updateSequentialUI();
}

void ConditionManagerEditor::newMessage(const ConditionManager::ConditionManagerEvent& e)
{
	if ((e.type == e.VALIDATION_CHANGED && e.multiplexIndex == conditionManager->getPreviewIndex())
		|| e.type == e.MULTIPLEX_PREVIEW_CHANGED)
	{
		contourColor = conditionManager->getIsValid(conditionManager->getPreviewIndex()) ? GREEN_COLOR : BG_COLOR.brighter(.3f);
		repaint();
	}
	else if (e.type == e.SEQUENTIAL_CONDITION_INDEX_CHANGED)
	{
		updateSequentialUI();
	}
}
