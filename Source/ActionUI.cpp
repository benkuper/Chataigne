/*
  ==============================================================================

    ActionUI.cpp
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionUI.h"

ActionUI::ActionUI(Action * _action) :
	ProcessorUI(_action),
	action(_action)
{
	bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);

	validUI = action->cdm.isValid->createToggle();
	validUI->showLabel = false;
	validUI->showValue = false;
	addAndMakeVisible(validUI);
}

ActionUI::~ActionUI()
{

}

void ActionUI::resizedInternalHeader(Rectangle<int>& r)
{
	validUI->setBounds(r.removeFromRight(headerHeight));
}
