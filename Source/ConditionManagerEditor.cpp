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
	GenericManagerEditor<Condition>(_manager, isRoot)
{
	addItemText = "Add Condition";
}

ConditionManagerEditor::~ConditionManagerEditor()
{
}

void ConditionManagerEditor::showMenuAndAddItem(bool)
{
	Condition *c = ConditionFactory::getInstance()->showCreateMenu();
	if (c != nullptr) manager->addItem(c);
	
}
