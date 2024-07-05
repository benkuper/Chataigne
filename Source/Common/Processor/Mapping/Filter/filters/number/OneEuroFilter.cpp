#include "Common/Processor/ProcessorIncludes.h"
/*
  ==============================================================================

	OneEuroFilter.cpp
	Created: 18 Jun 2021 10:45:02am
	Author:  bkupe

  ==============================================================================
*/

OneEuroMappingFilter::OneEuroMappingFilter(var params, Multiplex* multiplex) :
	TimeFilter(getTypeString(), params, multiplex)
{
	processOnSameValue = true;

	freq = 50;
	dcutoff = 1;

	mincutoff = addFloatParameter("Min Cutoff", "Minimum cutoff", .3f, 0);
	beta_ = addFloatParameter("Beta", "Beta", .1f, 0);

	filterTypeFilters.add(Controllable::BOOL, Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

OneEuroMappingFilter::~OneEuroMappingFilter()
{
}

float OneEuroMappingFilter::alpha(float cutoff)
{
	float te = 1.0f / freq;
	float tau = 1.0f / (2 * MathConstants<float>::pi * cutoff);
	return 1.0f / (1.0f + tau / te);
}

void OneEuroMappingFilter::setupParametersInternal(int mutiplexIndex, bool rangeOnly)
{
	allFilters.clear();
	filtersMap.clear();
	MappingFilter::setupParametersInternal(mutiplexIndex, rangeOnly);
}

Parameter* OneEuroMappingFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;
	if (!rangeOnly && source->type == Controllable::BOOL)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	}

	if (!filtersMap.contains(source))
	{

		int numDimensions = source->isComplex() ? source->value.size() : 1;
		OneEuroFilter* filter = new OneEuroFilter(numDimensions);
		allFilters.add(filter);
		filtersMap.set(source, filter);
	}

	return p;
}



MappingFilter::ProcessResult OneEuroMappingFilter::processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	if (source->checkValueIsTheSame(oldVal, newVal)) return UNCHANGED;



	for (auto& f : allFilters)
	{
		f->freq = freq;
		f->derivativeCutOff = dcutoff;
		f->minCutOff = mincutoff->floatValue();
		f->beta = beta_->floatValue();
	}

	//jassert(filtersMap.contains(source) && filtersMap[source] != nullptr);
	if (!filtersMap.contains(source) || filtersMap[source] == nullptr)
	{
		NLOGWARNING(niceName, "Filter not initialized");
		return STOP_HERE;
	}

	var val = filtersMap[source]->filter(oldVal, newVal, deltaTime);

	out->setValue(out->isComplex() ? val: val[0]);

	return CHANGED;
}


void OneEuroMappingFilter::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);
}