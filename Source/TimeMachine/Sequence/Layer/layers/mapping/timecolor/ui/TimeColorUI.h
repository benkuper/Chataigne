/*
  ==============================================================================

    TimeColorUI.h
    Created: 11 Apr 2017 11:40:31am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECOLORUI_H_INCLUDED
#define TIMECOLORUI_H_INCLUDED

#include "../TimeColor.h"

class TimeColorUI :
	public BaseItemMinimalUI<TimeColor>
{
public:
	TimeColorUI(TimeColor * item);
	~TimeColorUI();

	ScopedPointer<ColorParameterUI> colorUI;
	Path drawPath;

	float posAtMouseDown; 

	void paint(Graphics &g) override;
	void resized() override;

	void mouseDown(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
};



#endif  // TIMECOLORUI_H_INCLUDED
