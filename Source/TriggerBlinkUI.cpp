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
	intensity(0),
	animateIntensity(true),
blinkTime(100),
	offColor(NORMAL_COLOR),
onColor(FEEDBACK_COLOR),
refreshPeriod(50)
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
    if (showLabel)
	{
		g.setFont(10);
		g.setColour(Colours::white.darker(.1f));
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



void TriggerBlinkUI::mouseDownInternal(const MouseEvent&) {
    if(!forceFeedbackOnly) trigger->trigger();
}
