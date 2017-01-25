/*
  ==============================================================================

    ConditionManagerEditor.cpp
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManagerEditor.h"


ConditionManagerEditor::ConditionManagerEditor(ConditionManager * _manager, bool isRoot) :
	GenericManagerEditor<Condition>(_manager, isRoot)
{
	addItemText = "Add Condition";
}

ConditionManagerEditor::~ConditionManagerEditor()
{
}
