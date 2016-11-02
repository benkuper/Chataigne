/*
  ==============================================================================

    TripleSliderUI.h
    Created: 2 Nov 2016 4:17:34pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TRIPLESLIDERUI_H_INCLUDED
#define TRIPLESLIDERUI_H_INCLUDED


#include "ParameterUI.h"
#include "FloatSliderUI.h"
#include "Point3DParameter.h"
#include "FloatParameter.h"

class TripleSliderUI : public ParameterUI
{

public:
	TripleSliderUI(Point3DParameter * parameter = nullptr);
	virtual ~TripleSliderUI();

	enum Direction { HORIZONTAL, VERTICAL };

	Point3DParameter * p3d;

	FloatParameter xParam;
	FloatParameter yParam;
	FloatParameter zParam;

	FloatSliderUI xSlider;
	FloatSliderUI ySlider;
	FloatSliderUI zSlider;

	void setForceFeedbackOnlyInternal() override;
	void resized() override;

protected:
	virtual void newMessage(const Parameter::ParamWithValue & p) override;

	//void valueChanged(const var &) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TripleSliderUI)
};




#endif  // TRIPLESLIDERUI_H_INCLUDED
