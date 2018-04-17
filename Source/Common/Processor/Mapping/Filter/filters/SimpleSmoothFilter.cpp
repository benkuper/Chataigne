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
	smooth = filterParams.addFloatParameter("Smoothing", "Smooth amount of the filter. 0=no smoothing, 1=max smoothing (value will not change at all)", .5f, 0, 1);
}

SimpleSmoothFilter::~SimpleSmoothFilter()
{
}

void SimpleSmoothFilter::processInternal()
{
	float oldVal = filteredParameter->getNormalizedValue();
	float newVal = sourceParam->getNormalizedValue();
	filteredParameter->setNormalizedValue(oldVal + (newVal - oldVal)*(1 - smooth->floatValue()));}


