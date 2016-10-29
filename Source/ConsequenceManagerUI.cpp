/*
  ==============================================================================

    ConsequenceManagerUI.cpp
    Created: 28 Oct 2016 8:08:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceManagerUI.h"

ConsequenceManagerUI::ConsequenceManagerUI(ConsequenceManager * _manager) :
	BaseManagerUI<ConsequenceManager, Consequence, ConsequenceUI>("Consequences", _manager)
{
	autoSelectWithChildRespect = false;
	addItemText = "Add Consequence";
}

ConsequenceManagerUI::~ConsequenceManagerUI()
{
}
