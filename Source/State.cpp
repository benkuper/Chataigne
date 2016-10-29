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
	editorPosition = addPoint2DParameter("Position", "Position in State Machine view");
}

State::~State()
{
}
