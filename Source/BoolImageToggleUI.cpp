/*
  ==============================================================================

    BoolImageToggleUI.cpp
    Created: 4 Jan 2017 1:33:12pm
    Author:  Ben

  ==============================================================================
*/

#include "BoolImageToggleUI.h"

BoolImageToggleUI::BoolImageToggleUI(ImageButton * i, BoolParameter *p) :
	ParameterUI(p),
	boolParam(p),
	bt(i)
{
	addAndMakeVisible(bt);
	bt->addListener(this);
	bt->setToggleState(boolParam->boolValue(), dontSendNotification);
	bt->setTooltip(tooltip);
}

BoolImageToggleUI::~BoolImageToggleUI()
{
}

void BoolImageToggleUI::setTooltip(const String & value)
{
	ParameterUI::setTooltip(value);
	bt->setTooltip(value);
}

void BoolImageToggleUI::resized()
{
	bt->setBounds(getLocalBounds());
}

void BoolImageToggleUI::valueChanged(const var &)
{
	bt->setToggleState(boolParam->boolValue(), dontSendNotification);
}

void BoolImageToggleUI::buttonClicked(Button *)
{
	boolParam->setValue(!boolParam->boolValue());
}
