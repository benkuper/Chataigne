/*
  ==============================================================================

	ConditionGroupEditor.cpp
	Created: 21 Feb 2017 11:40:49am
	Author:  Ben

  ==============================================================================
*/

#include "ConditionGroupEditor.h"

ConditionGroupEditor::ConditionGroupEditor(ConditionGroup * _conditionGroup, bool isRoot) :
	ConditionEditor(_conditionGroup,isRoot),
	conditionGroup(_conditionGroup)
{
	conditionManagerEditor = (ConditionManagerEditor *)conditionGroup->manager.getEditor(false);
	addAndMakeVisible(conditionManagerEditor);
}

ConditionGroupEditor::~ConditionGroupEditor()
{
}

void ConditionGroupEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(conditionManagerEditor->getHeight());
	conditionManagerEditor->setBounds(r);
	r.setBottom(r.getBottom() + 2);
}
