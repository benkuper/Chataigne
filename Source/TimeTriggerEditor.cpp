/*
  ==============================================================================

    TimeTriggerEditor.cpp
    Created: 15 Oct 2017 5:34:27pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerEditor.h"

TimeTriggerEditor::TimeTriggerEditor(TimeTrigger * t, bool isRoot) :
	ActionEditor(t, isRoot),
	tt(t)
{
	timeEditor = tt->time->getEditor(false);
	addAndMakeVisible(timeEditor);
}

TimeTriggerEditor::~TimeTriggerEditor()
{

}

void TimeTriggerEditor::resizedInternalContent(Rectangle<int>& r)
{ 
	timeEditor->setBounds(r.withHeight(timeEditor->getHeight()));
	r.setY(timeEditor->getHeight()+5); 
	ActionEditor::resizedInternalContent(r);
}
