/*
  ==============================================================================

    SimpleSmoothFilter.cpp
    Created: 4 Feb 2017 5:39:34pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleSmoothFilter.h"

SimpleSmoothFilter::SimpleSmoothFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(),params, multiplex)
{
	processOnSameValue = true;

	async = filterParams.addBoolParameter("Asynchronous", "If enabled, you can have different rising smooth and falling smoothing values", false);
	smooth = filterParams.addFloatParameter("Smoothing", "Smooth amount of the filter. 0=no smoothing, 1=max smoothing (value will not change at all)", .5f, 0, 1);
	downSmooth = filterParams.addFloatParameter("Fall Smoothing", "If async, this is the smoothing when value is falling", .8f,0,1,false);

	filterTypeFilters.add(Controllable::BOOL, Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

SimpleSmoothFilter::~SimpleSmoothFilter()
{
}


Parameter* SimpleSmoothFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex)
{
	Parameter* p = nullptr;
	if (source->type == Controllable::BOOL)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex);
	}

	return p;
}

bool SimpleSmoothFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	if(source->checkValueIsTheSame(oldVal, newVal)) return false;

	var val;

	if (out->isComplex())
	{
		for (int i = 0; i < oldVal.size(); ++i)
		{
			float smoothVal = async->boolValue() ? (newVal[i] > oldVal[i] ? smooth->floatValue() : downSmooth->floatValue()) : smooth->floatValue();
			float targetVal = (float)oldVal[i] + ((float)newVal[i] - (float)oldVal[i]) * (1 - smoothVal);
			if (fabsf(targetVal - (float)newVal[i]) < precision) val.append(newVal[i]);
			else val.append(targetVal);
		}

	}
	else
	{
		float smoothVal = async->boolValue() ? (newVal > oldVal ? smooth->floatValue() : downSmooth->floatValue()) : smooth->floatValue();
		float targetVal = (float)oldVal + ((float)newVal - (float)oldVal) * (1 - smoothVal);
		
		if (fabsf(targetVal - (float)newVal) < precision) val = newVal;
		else val = targetVal;
	}

	out->setValue(val);

	return true;
}

void SimpleSmoothFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);

	if (c == async)
	{
		downSmooth->setEnabled(async->boolValue());
	}
 }