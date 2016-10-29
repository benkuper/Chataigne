/*
  ==============================================================================

    ConsequenceUI.cpp
    Created: 28 Oct 2016 8:08:09pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceUI.h"

ConsequenceUI::ConsequenceUI(Consequence * input) :
	BaseItemUI<Consequence>(input)
{
	autoSelectWithChildRespect = false;
	setSize(10, 40);
}

ConsequenceUI::~ConsequenceUI()
{

}
