/*
  ==============================================================================

    Action.cpp
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ActionEditor.h"

Action::Action() :
	BaseItem("Action")
{
	addChildControllableContainer(&cdm);
	addChildControllableContainer(&csm);
}

Action::~Action()
{
}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled) actionListeners.call(&Action::ActionListener::actionEnableChanged,this);
}

InspectorEditor * Action::getEditor()
{
	return new ActionEditor(this);
}
