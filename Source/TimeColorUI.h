/*
  ==============================================================================

    TimeColorUI.h
    Created: 11 Apr 2017 11:40:31am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECOLORUI_H_INCLUDED
#define TIMECOLORUI_H_INCLUDED

#include "TimeColor.h"
#include "BaseItemMinimalUI.h"

class TimeColorUI :
	public BaseItemMinimalUI<TimeColor>
{
public:
	TimeColorUI(TimeColor * item);
	~TimeColorUI();

	void paint(Graphics &g) override;

	void controllableFeedbackUpdateInternal(Controllable *c) override;
};



#endif  // TIMECOLORUI_H_INCLUDED
