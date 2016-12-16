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

	const static int handleSize = 6;

	void paint(Graphics &g) override;

	void mouseDown(const MouseEvent &e) override;

	//void mouseDrag(const MouseEvent &e) override;

};


#endif  // AUTOMATIONKEYUI_H_INCLUDED
