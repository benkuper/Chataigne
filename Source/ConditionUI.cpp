/*
  ==============================================================================

    ConditionUI.cpp
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionUI.h"



ConditionUI::ConditionUI(Condition * input) :
	BaseItemUI<Condition>(input)
{
	autoSelectWithChildRespect = false;

}

ConditionUI::~ConditionUI()
{

}
