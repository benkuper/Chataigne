/*
  ==============================================================================

    State.cpp
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "State.h"

State::State() :
	BaseItem("State")
{
	viewUIPosition = addPoint2DParameter("Position", "Position in State Machine view");
	viewUIPosition->setBounds(-100, -100,100,100);
	viewUIPosition->hideInOutliner = true;

	miniMode = addBoolParameter("Mini Mode", "Set the mini mode",false);
	miniMode->hideInOutliner = true;
	miniMode->hideInEditor = true;

	addChildControllableContainer(&am);
	addChildControllableContainer(&mm);
}

State::~State()
{
}
