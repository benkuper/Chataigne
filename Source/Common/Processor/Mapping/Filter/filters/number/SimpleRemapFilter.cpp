/*
  ==============================================================================

    SimpleRemapFilter.cpp
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleRemapFilter.h"


SimpleRemapFilter::SimpleRemapFilter(var params) :
	MappingFilter(getTypeString(),params),
	targetIn(nullptr),
	targetOut(nullptr)
{
	useCustomInputRange = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);
	
	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range",false);
	targetIn->setPoint(0, 1);

	targetOut = filterParams.addPoint2DParameter("Target Min/Max", "New maximum for output");
	targetOut->isCustomizableByUser = false;
	targetOut->setPoint(0, 1);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT);
	autoSetRange = false;
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

Parameter* SimpleRemapFilter::setupSingleParameterInternal(Parameter* source)
{
	Parameter* p = MappingFilter::setupSingleParameterInternal(source);
	if (!useCustomInputRange->isOverriden || !useCustomInputRange->boolValue()) useCustomInputRange->setValue(!source->hasRange());
	p->setRange(jmin<float>(targetOut->x, targetOut->y), jmax<float>(targetOut->x, targetOut->y));
	return p;
}

void SimpleRemapFilter::processSingleParameterInternal(Parameter * source, Parameter *out)
{
	if (targetIn == nullptr || targetOut == nullptr) return;
	
	float sourceVal = source->floatValue();
	float targetVal = sourceVal;
	if (targetOut->x == targetOut->y) targetVal = targetOut->x;

	if (useCustomInputRange->boolValue())
	{
		if (targetIn->x != targetIn->y && targetOut->x != targetOut->y)
		{
			float targetValue = jmap(source->floatValue(), targetIn->x, targetIn->y, targetOut->x, targetOut->y);
			if (targetOut->x > targetOut->y) targetValue = targetOut->y + (targetValue - targetOut->y) / (targetOut->x - targetOut->y);
			out->setValue(targetValue);
		}
	}
	else
	{
		float targetNValue = source->getNormalizedValue();
		if (targetOut->x > targetOut->y) targetNValue = 1 - targetNValue;
		out->setNormalizedValue(targetNValue);
	}
}



void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == useCustomInputRange)
	{
		targetIn->setEnabled(useCustomInputRange->boolValue());

	}else if (p == targetOut)
	{
		for(auto &f : filteredParameters)
		{
			if(f->type == Controllable::FLOAT) f->setRange(jmin<float>(targetOut->x,targetOut->y), jmax<float>(targetOut->x, targetOut->y));
		}
	}
}