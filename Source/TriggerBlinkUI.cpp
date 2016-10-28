/*
==============================================================================

TriggerBlinkUI.cpp
Created: 8 Mar 2016 3:45:53pm
Author:  bkupe

==============================================================================
*/


#include "TriggerBlinkUI.h"
#include "Style.h"
//==============================================================================
TriggerBlinkUI::TriggerBlinkUI(Trigger *t) :
	TriggerUI(t),
	blinkTime(100),
	refreshPeriod(50),
	intensity(0),
	animateIntensity(true),
	offColor(NORMAL_COLOR),
	onColor(HIGHLIGHT_COLOR)
{
    setSize(30, 20);

}

TriggerBlinkUI::~TriggerBlinkUI()
{

}

void TriggerBlinkUI::setTriggerReference(Trigger * t) {
    if (trigger != nullptr) {
        trigger->removeAsyncTriggerListener(this);
    }

    trigger = t;

    trigger->addAsyncTriggerListener(this);
}

void TriggerBlinkUI::triggerTriggered(const Trigger *) {
    startBlink();

}

void TriggerBlinkUI::paint(Graphics& g)
{
  if(!trigger.get())return;
	g.setColour(offColor.interpolatedWith(onColor,intensity));
    g.fillRoundedRectangle(getLocalBounds().toFloat(),2);
    g.setFont(10);
    g.setColour(Colours::white.darker(.1f));
	if (showLabel)
	{
		g.drawFittedText(trigger->niceName, getLocalBounds().reduced(2), Justification::centred, 1);
	}
}


void TriggerBlinkUI::startBlink(){
    intensity = 1;
	if (!animateIntensity) repaint();
	startTimer(animateIntensity ? refreshPeriod : blinkTime);

}

void TriggerBlinkUI::timerCallback(){

	if (animateIntensity)
	{
		intensity -= refreshPeriod*1.0f / blinkTime;

		if (intensity < 0) {
			intensity = 0;
			stopTimer();
		}
	} else
	{
		intensity = 0;
		stopTimer();
	}

    repaint();
}



void TriggerBlinkUI::mouseDown(const MouseEvent&) {
    trigger->trigger();

}
