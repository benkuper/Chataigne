/*
  ==============================================================================

    DoubleSliderUI.cpp
    Created: 25 Oct 2016 11:46:46am
    Author:  bkupe

  ==============================================================================
*/

#include "DoubleSliderUI.h"

DoubleSliderUI::DoubleSliderUI(Point2DParameter * parameter) :
	ParameterUI(parameter),
p2d(parameter),
	xParam("X","xParam",parameter->x, parameter->minimumValue[0],parameter->maximumValue[0]),
	yParam("Y", "yParam", parameter->y,parameter->minimumValue[0],parameter->maximumValue[0]),
	xSlider(&xParam), ySlider(&yParam)
{
	xParam.defaultValue = 0;
	yParam.defaultValue = 0;

	xParam.addAsyncCoalescedListener(this);
	yParam.addAsyncCoalescedListener(this);

	addAndMakeVisible(&xSlider);
	addAndMakeVisible(&ySlider);
}

DoubleSliderUI::~DoubleSliderUI()
{
	xParam.removeAsyncParameterListener(this);
	yParam.removeAsyncParameterListener(this);

}

void DoubleSliderUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	xSlider.setBounds(r.removeFromLeft(r.getWidth() / 2 - 5));
	ySlider.setBounds(r.removeFromRight(r.getWidth() - 10));
}


void DoubleSliderUI::setForceFeedbackOnlyInternal()
{
	xSlider.setForceFeedbackOnly(!parameter->isEditable || forceFeedbackOnly);
	ySlider.setForceFeedbackOnly(!parameter->isEditable || forceFeedbackOnly);

}

void DoubleSliderUI::newMessage(const Parameter::ParamWithValue & p)
{
	if (p.parameter == parameter)
	{
		xParam.setValue(((Point2DParameter *)p.parameter)->x);
		yParam.setValue(((Point2DParameter *)p.parameter)->y);

	} else if (p.parameter == &xParam)
	{
		if(xParam.floatValue() != p2d->x) p2d->setPoint(xParam.floatValue(), yParam.floatValue());
	} else if (p.parameter == &yParam)
	{
		if (yParam.floatValue() != p2d->y) p2d->setPoint(xParam.floatValue(), yParam.floatValue());
	} else if (p.isRange())
	{
		xParam.setRange(p.parameter->minimumValue[0], p.parameter->maximumValue[0]);
		yParam.setRange(p.parameter->minimumValue[1], p.parameter->maximumValue[1]);
	}
}
