/*
  ==============================================================================

    TripleSliderUI.cpp
    Created: 2 Nov 2016 4:17:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "TripleSliderUI.h"


TripleSliderUI::TripleSliderUI(Point3DParameter * parameter) :
	ParameterUI(parameter),
	xParam("X", "xParam", parameter->x, parameter->minX, parameter->maxX),
	yParam("Y", "yParam", parameter->y, parameter->minY, parameter->maxY),
	zParam("Z", "zParam", parameter->z, parameter->minZ, parameter->maxZ),
	xSlider(&xParam), ySlider(&yParam), zSlider(&zParam),
	p3d(parameter)
{
	xParam.defaultValue = 0;
	yParam.defaultValue = 0;
	xParam.addAsyncCoalescedListener(this);
	yParam.addAsyncCoalescedListener(this);

	addAndMakeVisible(&xSlider);
	addAndMakeVisible(&ySlider);
	addAndMakeVisible(&zSlider);
}

TripleSliderUI::~TripleSliderUI()
{
	xParam.removeAsyncParameterListener(this);
	yParam.removeAsyncParameterListener(this);
	zParam.removeAsyncParameterListener(this);

}

void TripleSliderUI::setForceFeedbackOnlyInternal()
{
	xSlider.setForceFeedbackOnly(!parameter->isEditable || forceFeedbackOnly);
	ySlider.setForceFeedbackOnly(!parameter->isEditable || forceFeedbackOnly);
	zSlider.setForceFeedbackOnly(!parameter->isEditable || forceFeedbackOnly);

}

void TripleSliderUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	xSlider.setBounds(r.removeFromLeft(r.getWidth() / 3 - 2));
	ySlider.setBounds(r.removeFromLeft(r.getWidth() /2 - 4));
	zSlider.setBounds(r.removeFromRight(r.getWidth() - 4));

}

void TripleSliderUI::newMessage(const Parameter::ParamWithValue & p)
{
	if (p.parameter == parameter)
	{
		xParam.setValue(((Point3DParameter *)p.parameter)->x);
		yParam.setValue(((Point3DParameter *)p.parameter)->y);
		zParam.setValue(((Point3DParameter *)p.parameter)->z);

	} else if (p.parameter == &xParam)
	{
		if (xParam.floatValue() != p3d->x) p3d->setVector(xParam.floatValue(), yParam.floatValue(),zParam.floatValue());
	} else if (p.parameter == &yParam)
	{
		if (yParam.floatValue() != p3d->y) p3d->setVector(xParam.floatValue(), yParam.floatValue(), zParam.floatValue());
	}else if (p.parameter == &zParam)
	{
		if (zParam.floatValue() != p3d->z) p3d->setVector(xParam.floatValue(), yParam.floatValue(), zParam.floatValue());
	} else if (p.isRange())
	{
		xParam.setRange(p.value[0], p.value[1]);
		yParam.setRange(p.value[2], p.value[3]);
		zParam.setRange(p.value[4], p.value[5]);
	}
}
