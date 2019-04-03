/*
  ==============================================================================

    LagFilter.cpp
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

#include "LagFilter.h"

LagFilter::LagFilter(var params) :
	MappingFilter(getTypeString())
{
	frequency = filterParams.addFloatParameter("Frequency", "Lag frequency in Hz", 5, .01f, 50);
	startTimerHz(frequency->floatValue());
}

LagFilter::~LagFilter()
{
}

void LagFilter::processInternal()
{
	filteredParameter->setValue(tempVal);
}

void LagFilter::filterParamChanged(Parameter * p)
{
	if(p == frequency) startTimerHz(frequency->floatValue());
}

void LagFilter::timerCallback()
{
	if (sourceParam == nullptr) return;
	tempVal = sourceParam->value;
}
