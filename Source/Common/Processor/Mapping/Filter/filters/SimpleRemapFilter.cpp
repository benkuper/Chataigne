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
	inputRangeFromSource = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);
	
	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range",false);
	targetIn->setPoint(0, 1);

	targetOut = filterParams.addPoint2DParameter("Target Min/Max", "New maximum for output");
	targetOut->isCustomizableByUser = false;
	targetOut->setPoint(0, 1);

	autoSetRange = false;
	forceOutParameterType = FloatParameter::getTypeStringStatic();
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

void SimpleRemapFilter::processInternal()
{
	if (inputRangeFromSource->boolValue())
	{
		filteredParameter->setValue(jmap(sourceParam->floatValue(), targetIn->x, targetIn->y, targetOut->x, targetOut->y));
	} else
	{
		filteredParameter->setNormalizedValue(sourceParam->getNormalizedValue()); 
	}
}


void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == inputRangeFromSource)
	{
		targetIn->setEnabled(inputRangeFromSource->boolValue());

	}else if (p == targetOut)
	{

		if(filteredParameter != nullptr) filteredParameter->setRange(targetOut->x, jmax<float>(targetOut->x,targetOut->y));
	}
}

Parameter * SimpleRemapFilter::setupParameterInternal(Parameter * source)
{ 
	Parameter * p = MappingFilter::setupParameterInternal(source);
	p->setRange(targetOut->x, jmax<float>(targetOut->x, targetOut->y));
	return p;
}
