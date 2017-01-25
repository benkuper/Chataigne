/*
  ==============================================================================

    ActionEditor.h
    Created: 25 Jan 2017 10:03:47am
    Author:  Ben

  ==============================================================================
*/

#ifndef ACTIONEDITOR_H_INCLUDED
#define ACTIONEDITOR_H_INCLUDED

#include "BaseItemEditor.h"
#include "ConditionManagerEditor.h"
#include "ConsequenceManagerEditor.h"
#include "Action.h"

class ActionEditor :
	public BaseItemEditor
{
public:
	ActionEditor(Action * a, bool isRoot);
	~ActionEditor();

	Action * action;
	ConditionManagerEditor cdme;
	ConsequenceManagerEditor csme;

	void resizedInternalContent(Rectangle<int> &r) override;
};


#endif  // ACTIONEDITOR_H_INCLUDED
