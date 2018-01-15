/*
  ==============================================================================

    SimpleRemapFilter.cpp
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleRemapFilter.h"

SimpleRemapFilter::SimpleRemapFilter(var params) :
	MappingFilter(getTypeString(),params)
{

	targetMin = filterParams.addFloatParameter("Target Min", "New minimum for output", 0, 0,1);
	targetMax = filterParams.addFloatParameter("Target Max", "New maximum for output", 1, 0,1);
	targetMin->defaultUI = FloatParameter::LABEL;
	targetMin->autoAdaptRange = true;
	targetMax->defaultUI = FloatParameter::LABEL;
	targetMax->autoAdaptRange = true;
	targetMin->isCustomizableByUser = false;
	targetMax->isCustomizableByUser = false;

	autoSetRange = false;
	forceOutParameterType = FloatParameter::getTypeStringStatic();
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

void SimpleRemapFilter::processInternal()
{
	filteredParameter->setNormalizedValue(sourceParam->getNormalizedValue());
}


void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == targetMin || p == targetMax)
	{

		if(filteredParameter != nullptr) filteredParameter->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(),targetMin->floatValue()));
	}
}

Parameter * SimpleRemapFilter::setupParameterInternal(Parameter * source)
{ 
	Parameter * p = MappingFilter::setupParameterInternal(source);
	p->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(), targetMin->floatValue()));
	return p;
}
