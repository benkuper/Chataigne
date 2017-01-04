/*
  ==============================================================================

    TriggerImageUI.cpp
    Created: 4 Jan 2017 1:32:47pm
    Author:  Ben

  ==============================================================================
*/

#include "TriggerImageUI.h"

TriggerImageUI::TriggerImageUI(Trigger * t, const Image &i) :
	TriggerUI(t),
	onImage(i),
	offImage(i.createCopy()),
	drawTriggering(false)
{
	offImage.desaturate();
	offImage.multiplyAllAlphas(.5f);
	setRepaintsOnMouseActivity(true);
}

TriggerImageUI::~TriggerImageUI()
{
	stopTimer();
}

void TriggerImageUI::paint(Graphics & g)
{
	g.drawImage(drawTriggering?onImage:offImage, getLocalBounds().toFloat());
}

void TriggerImageUI::triggerTriggered(const Trigger *)
{
	drawTriggering = true;
	repaint();
	startTimer(100);
}

void TriggerImageUI::timerCallback()
{
	drawTriggering = false;
	repaint();
	stopTimer();
}
