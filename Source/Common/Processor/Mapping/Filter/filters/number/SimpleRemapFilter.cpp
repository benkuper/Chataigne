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
	useCustomInputRange = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);
	
	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range",false);
	targetIn->setPoint(0, 1);

	targetOut = filterParams.addPoint2DParameter("Target Min/Max", "New maximum for output");
	targetOut->isCustomizableByUser = false;
	targetOut->setPoint(0, 1);

	autoSetRange = false;
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

void SimpleRemapFilter::processInternal()
{
	if (targetOut->x == targetOut->y)
	{
		if (filteredParameter->isComplex())
		{
			var val;
			for (int i = 0; i < filteredParameter->value.size(); i++) val.append(targetOut->x);
			filteredParameter->setValue(val);
		}
		else
		{
			filteredParameter->setValue(targetOut->x);
		}
		return;
	}

	if (useCustomInputRange->boolValue())
	{
		if (targetIn->x == targetIn->y)
		{
			if (filteredParameter->isComplex())
			{
				var val;
				for (int i = 0; i < filteredParameter->value.size(); i++) val.append(targetOut->x);
				filteredParameter->setValue(val);
			}
			else
			{
				filteredParameter->setValue(targetOut->x);
			}
			return;
		}
		
		if (filteredParameter->isComplex())
		{
			var val;
			for (int i = 0; i < filteredParameter->value.size(); i++)
			{
				float targetValue = jmap<float>(sourceParam->value[i], targetIn->x, targetIn->y, targetOut->x, targetOut->y);
				if (targetOut->x > targetOut->y) targetValue = targetOut->y + (targetValue - targetOut->y) / (targetOut->x - targetOut->y);
				val.append(targetValue);
			}
			filteredParameter->setValue(val);
		}
		else
		{
			float targetValue = jmap(sourceParam->floatValue(), targetIn->x, targetIn->y, targetOut->x, targetOut->y);
			if (targetOut->x > targetOut->y) targetValue = targetOut->y + (targetValue - targetOut->y) / (targetOut->x - targetOut->y);
			filteredParameter->setValue(targetValue);
		}
		
	} else
	{
		if (filteredParameter->isComplex())
		{
			var val;
			for (int i = 0; i < filteredParameter->value.size(); i++)
			{
				float targetNValue = ((float)sourceParam->value[i]- (float)sourceParam->minimumValue[i]) / ((float)sourceParam->maximumValue[i] - (float)sourceParam->minimumValue[i]);
				if (targetOut->x > targetOut->y) targetNValue = 1 - targetNValue;
				val.append(targetNValue);
			}
			val.append(val);
			filteredParameter->setValue(val);
		}
		else
		{
			float targetNValue = sourceParam->getNormalizedValue();
			if (targetOut->x > targetOut->y) targetNValue = 1 - targetNValue;
			filteredParameter->setNormalizedValue(targetNValue);
		}
	}
}


void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == useCustomInputRange)
	{
		targetIn->setEnabled(useCustomInputRange->boolValue());

	}else if (p == targetOut)
	{

		if (filteredParameter != nullptr)
		{
			filteredParameter->setRange(jmin<float>(targetOut->x,targetOut->y), jmax<float>(targetOut->x, targetOut->y));
		}
	}
}

Parameter * SimpleRemapFilter::setupParameterInternal(Parameter * source)
{ 
	Parameter * p = MappingFilter::setupParameterInternal(source);
	if(!useCustomInputRange->isOverriden) useCustomInputRange->setValue(!source->hasRange());
	p->setRange(targetOut->x, jmax<float>(targetOut->x, targetOut->y));
	return p;
}
