/*
  ==============================================================================

    ScriptConditionEditor.cpp
    Created: 21 Feb 2017 11:40:39am
    Author:  Ben

  ==============================================================================
*/

#include "ScriptConditionEditor.h"

ScriptConditionEditor::ScriptConditionEditor(ScriptCondition * s, bool isRoot) :
	ConditionEditor(s, isRoot),
	scriptCondition(s)
{
	scriptUI = new ScriptUI(&scriptCondition->script);
	addAndMakeVisible(scriptUI);
}

ScriptConditionEditor::~ScriptConditionEditor()
{
}

void ScriptConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(scriptUI->getHeight());
	scriptUI->setBounds(r);
}

