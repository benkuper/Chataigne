/*
  ==============================================================================

    LagFilter.cpp
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

#include "LagFilter.h"


LagFilter::LagFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	frequency = filterParams.addFloatParameter("Frequency", "Lag frequency in Hz", 5, .01f, 50);
	startTimerHz(frequency->floatValue());
}

LagFilter::~LagFilter()
{
}

void LagFilter::setupParametersInternal(int multiplexIndex)
{
	paramTempValueMap.clear();
	MappingFilter::setupParametersInternal(multiplexIndex);
}

Parameter* LagFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex)
{
	var tmpVal = var(source->getValue()); //shoud maybe copy the values or is it enough ?
	paramTempValueMap.set(source, tmpVal);
	return MappingFilter::setupSingleParameterInternal(source, multiplexIndex);
}

bool LagFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!paramTempValueMap.contains(source)) return false;
	if (paramTempValueMap[source] == out->getValue()) return false;
	
	out->setValue(paramTempValueMap[source]);
	return true;
}

void LagFilter::filterParamChanged(Parameter * p)
{
	if(p == frequency) startTimerHz(frequency->floatValue());
}

void LagFilter::timerCallback()
{
	for (auto& mSourceParams : sourceParams)
	{
		for (auto& s : mSourceParams)
		{
			if (s == nullptr) continue;
			paramTempValueMap.set(s, var(s->value));
		}
	}
}
