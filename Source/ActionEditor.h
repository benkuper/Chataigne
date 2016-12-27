/*
  ==============================================================================

    ActionEditor.h
    Created: 29 Oct 2016 5:52:20pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONEDITOR_H_INCLUDED
#define ACTIONEDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "ActionUI.h"
#include "ConditionManagerUI.h"
#include "ConsequenceManagerUI.h"

class ActionEditor :
	public InspectableEditor
{
public:
	ActionEditor(Action * _action, bool isRoot);
	~ActionEditor();

	Action * action;

	ConditionManagerUI cdmui;
	ConsequenceManagerUI csmui;

	void resized() override;

};


#endif  // ACTIONEDITOR_H_INCLUDED
