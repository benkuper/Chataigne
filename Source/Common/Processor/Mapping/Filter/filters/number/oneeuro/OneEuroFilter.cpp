/*
  ==============================================================================

	OneEuroFilter.cpp
	Created: 18 Jun 2021 10:45:02am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

OneEuroFilter::OneEuroFilter(int dimensions) :
	dimensions(dimensions)
{
	for (int i = 0; i < dimensions; i++)
	{
		x.add(new LowPassFilter());
		dx.add(new LowPassFilter());
	}

	freq = 50;
	minCutOff = 1;
	beta = 10;
	derivativeCutOff = 1;
}

OneEuroFilter::~OneEuroFilter()
{
}

float OneEuroFilter::alpha(float cutoff)
{
	float te = 1.0f / freq;
	float tau = 1.0f / (2 * MathConstants<float>::pi * cutoff);
	return 1.0f / (1.0f + tau / te);
}


Array<float> OneEuroFilter::filter(Array<float> oldValues, Array<float> newValues, double deltaTime)
{
	freq = 1.0 / deltaTime;

	Array<float> result;

	for (int i = 0; i < dimensions; i++)
	{
		float deltaP = (newValues[i] - oldValues[i]) * freq;

		float edvalue = dx[i]->filterWithAlpha(deltaP, alpha(derivativeCutOff));
		float cutoff = minCutOff + beta * fabs(edvalue);
		result.add(x[i]->filterWithAlpha(newValues[i], alpha(cutoff)));
	}

	return result;
}


var OneEuroFilter::filter(var oldPos, var newPos, double deltaTime)
{
	Array<float> oldValues;
	Array<float> newValues;

	if (newPos.isArray())
	{
		for (int i = 0; i < newPos.size(); i++)
		{
			oldValues.add(oldPos[i]);
			newValues.add(newPos[i]);
		}
	}
	else
	{
		oldValues.add(oldPos);
		newValues.add(newPos);
	}

	jassert(oldValues.size() == newValues.size());

	Array<float> p = filter(oldValues, newValues, deltaTime);

	var result;
	for (int i = 0; i < p.size(); i++) result.append(p[i]);

	jassert(result.size() == oldValues.size());

	return result;
}


OneEuroFloatFilter::OneEuroFloatFilter() :
	OneEuroFilter(1)
{
}

OneEuroFloatFilter::~OneEuroFloatFilter()
{
}

float OneEuroFloatFilter::filter(float oldValue, float newValue, double deltaTime)
{
	Array<float> p = OneEuroFilter::filter(Array<float>{ oldValue }, Array<float>{ newValue }, deltaTime);
	return p[0];
}