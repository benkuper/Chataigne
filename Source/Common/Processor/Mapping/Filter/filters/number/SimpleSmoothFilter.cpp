/*
  ==============================================================================

	SimpleSmoothFilter.cpp
	Created: 4 Feb 2017 5:39:34pm
	Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

SimpleSmoothFilter::SimpleSmoothFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true)
{
	processOnSameValue = true;

	async = filterParams.addBoolParameter("Asynchronous", "If enabled, you can have different rising smooth and falling smoothing values", false);
	smooth = filterParams.addFloatParameter("Smoothing", "Smooth amount of the filter. 0=no smoothing, 1=max smoothing (value will not change at all)", .5f, 0, 1);
	downSmooth = filterParams.addFloatParameter("Fall Smoothing", "If async, this is the smoothing when value is falling", .8f, 0, 1, false);

	filterTypeFilters.add(Controllable::BOOL, Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

SimpleSmoothFilter::~SimpleSmoothFilter()
{
}


Parameter* SimpleSmoothFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;
	if (!rangeOnly && source->type == Controllable::BOOL)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	}

	return p;
}

MappingFilter::ProcessResult SimpleSmoothFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	if (source->checkValueIsTheSame(oldVal, newVal)) return UNCHANGED;

	var val;

	float upSmoothVal = filterParams.getLinkedValue(smooth, multiplexIndex);
	bool asyncVal = ((bool)filterParams.getLinkedValue(async, multiplexIndex));
	float downSmoothVal = asyncVal ? (float)filterParams.getLinkedValue(downSmooth, multiplexIndex) : upSmoothVal;

	if (out->isComplex())
	{
		for (int i = 0; i < oldVal.size(); ++i)
		{
			float smoothVal = (newVal[i] > oldVal[i]) ? upSmoothVal : downSmoothVal;
			float targetVal = (float)oldVal[i] + ((float)newVal[i] - (float)oldVal[i]) * (1 - smoothVal);
			if (fabsf(targetVal - (float)newVal[i]) < precision) val.append(newVal[i]);
			else val.append(targetVal);
		}

	}
	else
	{
		float smoothVal = (newVal > oldVal) ? upSmoothVal : downSmoothVal;
		float targetVal = (float)oldVal + ((float)newVal - (float)oldVal) * (1 - smoothVal);

		if (fabsf(targetVal - (float)newVal) < precision) val = newVal;
		else val = targetVal;
		if (isnan((float)val))
		{
			jassertfalse;
			return UNCHANGED;
		}
	}

	out->setValue(val);

	return CHANGED;
}

void SimpleSmoothFilter::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);

	if (c == async)
	{
		downSmooth->setEnabled(async->boolValue());
	}
}