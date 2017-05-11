/*
==============================================================================

TriggerBlinkUI.h
Created: 8 Mar 2016 3:45:53pm
Author:  bkupe

==============================================================================
*/

#ifndef TRIGGERBLINKUI_H_INCLUDED
#define TRIGGERBLINKUI_H_INCLUDED



//==============================================================================
/*
*/


#include "TriggerUI.h"

class TriggerBlinkUI : public TriggerUI, public Timer
{
public:
    TriggerBlinkUI(Trigger * t);
    virtual ~TriggerBlinkUI();

    void paint(Graphics&)override;
    void mouseDownInternal(const MouseEvent& event)override;
    void triggerTriggered(const Trigger * p) override;
    void startBlink();
    void timerCallback()override;
    void setTriggerReference(Trigger * t);
    float intensity;

	bool animateIntensity;
	int blinkTime;
	Colour offColor;
	Colour onColor;
private:

    int refreshPeriod;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerBlinkUI)
};


#endif  // TRIGGERBLINKUI_H_INCLUDED
