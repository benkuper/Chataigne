/*
  ==============================================================================

    SimpleSmoothFilter.cpp
    Created: 4 Feb 2017 5:39:34pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleSmoothFilter.h"


SimpleSmoothFilter::SimpleSmoothFilter(var params) :
	MappingFilter(getTypeString())
{
	needsContinuousProcess = true;
	forceOutParameterType = FloatParameter::getTypeStringStatic();
	async = filterParams.addBoolParameter("Asynchronous", "If enabled, you can have different rising smooth and falling smoothing values", false);
	smooth = filterParams.addFloatParameter("Smoothing", "Smooth amount of the filter. 0=no smoothing, 1=max smoothing (value will not change at all)", .5f, 0, 1);
	downSmooth = filterParams.addFloatParameter("Fall Smoothing", "If async, this is the smoothing when value is falling", .8f,0,1,false);
}

SimpleSmoothFilter::~SimpleSmoothFilter()
{
}

void SimpleSmoothFilter::processInternal()
{
	float oldVal = filteredParameter->getValue();
	float newVal = sourceParam->getValue();
	float smoothVal = async->boolValue() ? (newVal > oldVal ? smooth->floatValue() : downSmooth->floatValue()) : smooth->floatValue();
	filteredParameter->setValue(oldVal + (newVal - oldVal)*(1 - smoothVal));
}

void SimpleSmoothFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);

	if (c == async)
	{
		downSmooth->setEnabled(async->boolValue());
	}
}


