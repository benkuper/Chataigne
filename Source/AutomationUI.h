/*
  ==============================================================================

    AutomationUI.h
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONUI_H_INCLUDED
#define AUTOMATIONUI_H_INCLUDED

#include "Automation.h"
#include "BaseManagerUI.h"
#include "AutomationKeyUI.h"

class AutomationUI :
	public BaseManagerUI<Automation,AutomationKey,AutomationKeyUI>
{
public:
	AutomationUI(Automation * _automation);
	virtual ~AutomationUI();
	
	void resized() override;

	void mouseDown(const MouseEvent &e) override;
};



#endif  // AUTOMATIONUI_H_INCLUDED
