/*
  ==============================================================================

    AutomationKeyUI.h
    Created: 11 Dec 2016 1:22:27pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONKEYUI_H_INCLUDED
#define AUTOMATIONKEYUI_H_INCLUDED

#include "AutomationKey.h"
#include "BaseItemMinimalUI.h"

class AutomationKeyUI :
	public BaseItemMinimalUI<AutomationKey>
{
public:
	AutomationKeyUI(AutomationKey *);
	virtual ~AutomationKeyUI();
	
	void paint(Graphics &g) override;
	void resized() override;
};




#endif  // AUTOMATIONKEYUI_H_INCLUDED
