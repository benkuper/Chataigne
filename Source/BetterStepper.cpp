/*
  ==============================================================================

    BetterIntStepper.cpp
    Created: 28 Sep 2016 3:14:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "BetterStepper.h"

BetterStepper::BetterStepper(const String &tooltip) : Slider(SliderStyle::IncDecButtons,TextEntryBoxPosition::TextBoxLeft)
{
	setTooltip(tooltip);
	setIncDecButtonsMode(IncDecButtonMode::incDecButtonsDraggable_AutoDirection);
}

BetterStepper::~BetterStepper()
{
}

void BetterStepper::setTooltip(const String & tooltip)
{
	Slider::setTooltip(tooltip);
}
