/*
  ==============================================================================

    StateUI.cpp
    Created: 28 Oct 2016 8:19:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateUI.h"
StateUI::StateUI(State * state) :
	BaseItemUI<State>(state)
{
}

StateUI::~StateUI()
{
	DBG("StateUI destructor");
}
