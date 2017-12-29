/*
  ==============================================================================

    ConditionManagerEditor.cpp
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManagerEditor.h"
#include "ConditionFactory.h"

ConditionManagerEditor::ConditionManagerEditor(ConditionManager * _manager, bool isRoot) :
	GenericManagerEditor<Condition>(_manager, isRoot),
	conditionManager(_manager)
{
	addItemText = "Add Condition";
	noItemText = "Add a triggering condition here";
	
}

ConditionManagerEditor::~ConditionManagerEditor()
{
}

void ConditionManagerEditor::showMenuAndAddItem(bool)
{
	Condition *c = ConditionFactory::getInstance()->showCreateMenu();
	if (c != nullptr) manager->addItem(c);
	
}

void ConditionManagerEditor::itemAdded(Condition *)
{
	if (manager->items.size() > 1) resetAndBuild();
}

void ConditionManagerEditor::itemRemoved(Condition *)
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
