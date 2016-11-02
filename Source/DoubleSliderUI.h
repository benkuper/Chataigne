/*
  ==============================================================================

    DoubleSliderUI.h
    Created: 25 Oct 2016 11:46:46am
    Author:  bkupe

  ==============================================================================
*/

#ifndef DOUBLESLIDERUI_H_INCLUDED
#define DOUBLESLIDERUI_H_INCLUDED


#include "ParameterUI.h"
#include "FloatSliderUI.h"
#include "Point2DParameter.h"
#include "FloatParameter.h"

class DoubleSliderUI : public ParameterUI
{

public:
	DoubleSliderUI(Point2DParameter * parameter = nullptr);
	virtual ~DoubleSliderUI();

	enum Direction { HORIZONTAL, VERTICAL };
	
	Point2DParameter * p2d;

	FloatParameter xParam;
	FloatParameter yParam;

	FloatSliderUI xSlider;
	FloatSliderUI ySlider;

    void resized() override;

protected:
	void setForceFeedbackOnlyInternal() override;
	virtual void newMessage(const Parameter::ParamWithValue & p) override;

	//void valueChanged(const var &) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DoubleSliderUI)
};



#endif  // DOUBLESLIDERUI_H_INCLUDED
