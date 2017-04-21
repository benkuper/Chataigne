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
#include "ColorParameterUI.h"

class TimeColorUI :
	public BaseItemMinimalUI<TimeColor>,
	public KeyListener
{
public:
	TimeColorUI(TimeColor * item);
	~TimeColorUI();

	ScopedPointer<ColorParameterUI> colorUI;
	Path drawPath;

	void paint(Graphics &g) override;
	void resized() override;

	void mouseDown(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable *c) override;

	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};



#endif  // TIMECOLORUI_H_INCLUDED
