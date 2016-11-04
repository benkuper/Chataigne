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
	removeMouseListener(this);

}

ConsequenceUI::~ConsequenceUI()
{

}

void ConsequenceUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI::mouseDown(e);
	if (e.mods.isRightButtonDown())
	{
		CommandDefinition * d = CommandFactory::showMenuAndGetCommand();
		if (d != nullptr)
		{
			item->setCommand(d);
		}
	}
}
