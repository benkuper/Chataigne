/*
  ==============================================================================

    TriggerUI.h
    Created: 8 Mar 2016 3:48:52pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TRIGGERUI_H_INCLUDED
#define TRIGGERUI_H_INCLUDED

#include "ControllableUI.h"
#include "Trigger.h"

class TriggerUI : 
	public ControllableUI, 
	public Trigger::AsyncListener
{
public :
    TriggerUI(Trigger * trigger);
    virtual ~TriggerUI();

    WeakReference<Trigger>  trigger;

    // Inherited via AsyncListener
	void newMessage(const WeakReference<Trigger> &) override;
	
	// should be inherited
    virtual void triggerTriggered(const Trigger * p) =0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerUI)


};


#endif  // TRIGGERUI_H_INCLUDED
