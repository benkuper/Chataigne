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
	async = filterParams.addBoolParameter("Asynchronous", "If enabled, you can have different rising smooth and falling smoothing values", false);
	smooth = filterParams.addFloatParameter("Smoothing", "Smooth amount of the filter. 0=no smoothing, 1=max smoothing (value will not change at all)", .5f, 0, 1);
	downSmooth = filterParams.addFloatParameter("Fall Smoothing", "If async, this is the smoothing when value is falling", .8f,0,1,false);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

SimpleSmoothFilter::~SimpleSmoothFilter()
{
}



void SimpleSmoothFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	if (out->isComplex())
	{
		var val;
		for (int i = 0; i < out->value.size(); i++)
		{
			float smoothVal = async->boolValue() ? (newVal[i] > oldVal[i] ? smooth->floatValue() : downSmooth->floatValue()) : smooth->floatValue();
			val.append((float)oldVal[i] + ((float)newVal[i] - (float)oldVal[i]) * (1 - smoothVal));
		}

		out->setValue(val);
	}
	else
	{
		float smoothVal = async->boolValue() ? (newVal > oldVal ? smooth->floatValue() : downSmooth->floatValue()) : smooth->floatValue();
		out->setValue((float)oldVal + ((float)newVal - (float)oldVal) * (1 - smoothVal));
	}
}

void SimpleSmoothFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);

	if (c == async)
	{
		downSmooth->setEnabled(async->boolValue());
	}
 }