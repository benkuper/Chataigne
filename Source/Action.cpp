/*
  ==============================================================================

    Action.cpp
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"


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
