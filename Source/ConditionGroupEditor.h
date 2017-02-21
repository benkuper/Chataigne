/*
  ==============================================================================

    ConditionGroupEditor.h
    Created: 21 Feb 2017 11:40:49am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONDITIONGROUPEDITOR_H_INCLUDED
#define CONDITIONGROUPEDITOR_H_INCLUDED

#include "ConditionEditor.h"
#include "ConditionManagerEditor.h"
#include "ConditionGroup.h"

class ConditionGroupEditor :
	public ConditionEditor
{
public:
	ConditionGroupEditor(ConditionGroup * conditionGroup, bool isRoot);
	~ConditionGroupEditor();

	ConditionGroup * conditionGroup;
	ScopedPointer<ConditionManagerEditor> conditionManagerEditor;

	void resizedInternalContent(Rectangle<int> &r) override;
};


#endif  // CONDITIONGROUPEDITOR_H_INCLUDED
