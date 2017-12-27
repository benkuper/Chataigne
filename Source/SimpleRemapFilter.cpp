/*
  ==============================================================================

    SimpleRemapFilter.cpp
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleRemapFilter.h"

SimpleRemapFilter::SimpleRemapFilter() :
	MappingFilter("Remap")
{
	targetMin = filterParams.addFloatParameter("Target Min", "New minimum for output", 0, -1000, 1000);
	targetMax = filterParams.addFloatParameter("Target Max", "New maximum for output", 1, -1000, 1000);
	autoSetRange = false;
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

void SimpleRemapFilter::processInternal()
{
	filteredParameter->setNormalizedValue(sourceParam->getNormalizedValue());
}

Parameter * SimpleRemapFilter::setupParameterInternal(Parameter * source, const String &forceType)
{
	Parameter * p = MappingFilter::setupParameterInternal(source,FloatParameter::getTypeStringStatic());
	p->setRange(targetMin->floatValue(), targetMax->floatValue());
	return p;
}

void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == targetMin || p == targetMax)
	{

		if(filteredParameter != nullptr) filteredParameter->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(),targetMin->floatValue()));
	}
}

