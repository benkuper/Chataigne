/*
  ==============================================================================

    ScriptConditionEditor.h
    Created: 21 Feb 2017 11:40:39am
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTCONDITIONEDITOR_H_INCLUDED
#define SCRIPTCONDITIONEDITOR_H_INCLUDED

#include "ConditionEditor.h"
#include "ScriptCondition.h"

class ScriptConditionEditor :
	public ConditionEditor
{
public:
	ScriptConditionEditor(ScriptCondition * s, bool isRoot);
	~ScriptConditionEditor();

	ScriptCondition * scriptCondition;
	ScopedPointer<ScriptEditor> scriptEditor;

	void resizedInternalContent(Rectangle<int> &r) override;
};



#endif  // SCRIPTCONDITIONEDITOR_H_INCLUDED
