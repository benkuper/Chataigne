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
	scriptEditor = static_cast<ScriptEditor *>(scriptCondition->script.getEditor(false));
	addAndMakeVisible(scriptEditor);
	scriptEditor->transparentBG = true;
}

ScriptConditionEditor::~ScriptConditionEditor()
{
}

void ScriptConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(scriptEditor->getHeight());
	scriptEditor->setBounds(r);
}

