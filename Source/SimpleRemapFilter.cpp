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

	targetMin = filterParams.addFloatParameter("Target Min", "New minimum for output", 0, 0, 1);
	targetMin->isCustomizableByUser = true;
	targetMin->saveValueOnly = false;
	targetMax = filterParams.addFloatParameter("Target Max", "New maximum for output", 1, 0, 1);
	targetMax->isCustomizableByUser = true;
	targetMax->saveValueOnly = false;
	
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

