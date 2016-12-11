/*
  ==============================================================================

    AutomationKeyUI.cpp
    Created: 11 Dec 2016 1:22:27pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationKeyUI.h"

AutomationKeyUI::AutomationKeyUI(AutomationKey * key) :
	BaseItemMinimalUI(key)
{
}

AutomationKeyUI::~AutomationKeyUI()
{
}

void AutomationKeyUI::paint(Graphics & g)
{
	g.setColour(GREEN_COLOR);
	g.fillEllipse(getLocalBounds().toFloat());
}

void AutomationKeyUI::resized()
{
}
