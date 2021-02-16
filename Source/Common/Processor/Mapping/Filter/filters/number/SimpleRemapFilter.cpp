/*
  ==============================================================================

    SimpleRemapFilter.cpp
    Created: 4 Feb 2017 5:39:07pm
    Author:  Ben

  ==============================================================================
*/

#include "SimpleRemapFilter.h"


SimpleRemapFilter::SimpleRemapFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex),
	targetIn(nullptr),
	targetOut(nullptr)
{
	useCustomInputRange = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);
	
	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range",false);
	targetIn->setPoint(0, 1);

	targetOut = filterParams.addPoint2DParameter("Target Min/Max", "New maximum for output");
	targetOut->isCustomizableByUser = false;
	targetOut->setPoint(0, 1);

	forceFloatOutput = filterParams.addBoolParameter("Force Float", "If checked, this will force transforming integer in floats", false);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::BOOL);
	autoSetRange = false;
}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

Parameter* SimpleRemapFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex)
{
	Parameter* p = nullptr;
	if (forceFloatOutput->boolValue())
	{
		p = new FloatParameter(source->niceName, source->description, source->value, source->minimumValue, source->maximumValue);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex);
	}

	if (!useCustomInputRange->isOverriden || !useCustomInputRange->boolValue()) useCustomInputRange->setValue(!source->hasRange());
	p->setRange(jmin<float>(targetOut->x, targetOut->y), jmax<float>(targetOut->x, targetOut->y));
	return p;
}

MappingFilter::ProcessResult  SimpleRemapFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (targetIn == nullptr || targetOut == nullptr || out == nullptr) return STOP_HERE;
	
	float sourceVal = source->floatValue();
	float targetVal = sourceVal;

	var tOut = filterParams.getLinkedValue(targetOut, multiplexIndex);
	var tIn = filterParams.getLinkedValue(targetIn, multiplexIndex);

	if (tOut[0] == tOut[1]) targetVal = tOut[0];

	if (useCustomInputRange->boolValue())
	{
		if (tIn[0] != tIn[1] && tOut[0] != tOut[1])
		{
			float targetValue = jmap(source->floatValue(), (float)tIn[0], (float)tIn[1], (float)tOut[0], (float)tOut[1]);
			//if (tOut[0] > tOut[1]) targetValue = tOut[1] + (targetValue - targetOut->y) / (tOut[0] - targetOut->y);
			out->setValue(targetValue);
		}
	}
	else
	{
		float targetNValue = source->getNormalizedValue();
		if ((float)tOut[0] > (float)tOut[1]) targetNValue = 1 - targetNValue;
		out->setNormalizedValue(targetNValue);
	}

	return CHANGED;
}



void SimpleRemapFilter::filterParamChanged(Parameter * p)
{
	if (p == useCustomInputRange)
	{
		targetIn->setEnabled(useCustomInputRange->boolValue());

	}else if (p == targetOut)
	{
		for(auto &mFilteredParams : filteredParameters)
		{
			for (auto& f : *mFilteredParams)
			{
				if (f->type == Controllable::FLOAT || f->type == Controllable::INT) f->setRange(jmin<float>(targetOut->x, targetOut->y), jmax<float>(targetOut->x, targetOut->y));
			}
		}

		mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
	}
	else if (p == forceFloatOutput)
	{
		if(!isCurrentlyLoadingData) setupParametersInternal(-1);

		mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
		filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}
}