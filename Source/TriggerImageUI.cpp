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
	if(trigger->isControllableFeedbackOnly) offImage.multiplyAllAlphas(.5f);
	setRepaintsOnMouseActivity(true);
}

TriggerImageUI::~TriggerImageUI()
{
	stopTimer();
}

void TriggerImageUI::paint(Graphics & g)
{
	
	g.drawImage(drawTriggering?onImage:offImage, getLocalBounds().toFloat());
	if (isMouseOver() && !trigger->isControllableFeedbackOnly)
	{
		g.setColour(HIGHLIGHT_COLOR.withAlpha(.2f));
		g.fillEllipse(getLocalBounds().reduced(2).toFloat());
	}
}

void TriggerImageUI::triggerTriggered(const Trigger *)
{
	drawTriggering = true;
	repaint();
	startTimer(100);
}

void TriggerImageUI::mouseDown(const MouseEvent &)
{
	trigger->trigger();
}

void TriggerImageUI::timerCallback()
{
	drawTriggering = false;
	repaint();
	stopTimer();
}
