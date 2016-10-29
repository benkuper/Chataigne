/*
  ==============================================================================

    ConditionManagerUI.cpp
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManagerUI.h"


ConditionManagerUI::ConditionManagerUI(ConditionManager * _manager) :
	BaseManagerUI<ConditionManager, Condition, ConditionUI>("Conditions", _manager)
{
	autoSelectWithChildRespect = false;
	addItemText = "Add Condition";
}

ConditionManagerUI::~ConditionManagerUI()
{
}
