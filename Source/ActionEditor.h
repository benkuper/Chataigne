/*
  ==============================================================================

    ActionEditor.h
    Created: 29 Oct 2016 5:52:20pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONEDITOR_H_INCLUDED
#define ACTIONEDITOR_H_INCLUDED

#include "CustomEditor.h"
#include "ActionUI.h"
#include "ConditionManagerUI.h"
#include "ConsequenceManagerUI.h"

class ActionEditor :
	public CustomEditor
{
public:
	ActionEditor(ActionUI * _action);
	~ActionEditor();

	ActionUI * actionUI;
	Action * action;

	ConditionManagerUI cdmui;
	ConsequenceManagerUI csmui;


	void resized() override;

	int getContentHeight() override;
};


#endif  // ACTIONEDITOR_H_INCLUDED
