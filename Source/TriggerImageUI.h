/*
  ==============================================================================

    TriggerImageUI.h
    Created: 4 Jan 2017 1:32:47pm
    Author:  Ben

  ==============================================================================
*/

#ifndef TRIGGERIMAGEUI_H_INCLUDED
#define TRIGGERIMAGEUI_H_INCLUDED

#include "Trigger.h"
#include "TriggerUI.h"

class TriggerImageUI :
	public TriggerUI,
	public Timer
{
public:
	TriggerImageUI(Trigger *, const Image &image);
	~TriggerImageUI();

	Image onImage;
	Image offImage;

	bool drawTriggering;

	void paint(Graphics &g) override;
	void triggerTriggered(const Trigger * p) override;

	void mouseDown(const MouseEvent &e) override;


	// Inherited via Timer
	virtual void timerCallback() override;
};


#endif  // TRIGGERIMAGEUI_H_INCLUDED
