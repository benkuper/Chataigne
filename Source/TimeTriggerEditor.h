/*
  ==============================================================================

    TimeTriggerEditor.h
    Created: 15 Oct 2017 5:34:27pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "ActionEditor.h"
#include "TimeTrigger.h"

class TimeTriggerEditor :
	public ActionEditor
{
public:
	TimeTriggerEditor(TimeTrigger * t,bool isRoot = false);
	~TimeTriggerEditor();

	TimeTrigger * tt;

	ScopedPointer<InspectableEditor> timeEditor;

	void resizedInternalContent(Rectangle<int> &r) override;
};