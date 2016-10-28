/*
  ==============================================================================

    TriggerButtonUI.h
    Created: 8 Mar 2016 3:45:53pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TRIGGERBUTTONUI_H_INCLUDED
#define TRIGGERBUTTONUI_H_INCLUDED



//==============================================================================
/*
*/


#include "TriggerUI.h"

class TriggerButtonUI : public TriggerUI
{
public:
    TriggerButtonUI(Trigger * t);
    ~TriggerButtonUI();

    ComponentAnimator animator;

    void paint (Graphics&) override;
    void resized() override;
    void mouseDown (const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;
    void mouseEnter(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void triggerTriggered(const Trigger * p) override ;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggerButtonUI)
};


#endif  // TRIGGERBUTTONUI_H_INCLUDED
