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

void LagFilter::setupParametersInternal()
{
	paramTempValueMap.clear();
	MappingFilter::setupParametersInternal();
}

Parameter* LagFilter::setupSingleParameterInternal(Parameter* source)
{
	var tmpVal = var(source->getValue()); //shoud maybe copy the values or is it enough ?
	paramTempValueMap.set(source, tmpVal);
	return MappingFilter::setupSingleParameterInternal(source);
}

void LagFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	if (!paramTempValueMap.contains(source)) return;
	out->setValue(paramTempValueMap[source]);
}

void LagFilter::filterParamChanged(Parameter * p)
{
	if(p == frequency) startTimerHz(frequency->floatValue());
}

void LagFilter::timerCallback()
{
	for (auto& s : sourceParams)
	{
		if (s == nullptr) continue;
		paramTempValueMap.set(s, var(s->value));
	}
}
