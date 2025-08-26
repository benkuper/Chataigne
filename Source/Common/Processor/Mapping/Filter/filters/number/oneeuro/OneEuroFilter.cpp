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

double OneEuroFilter::alpha(double cutoff)
{
	double te = 1.0f / freq;
	double tau = 1.0f / (2 * MathConstants<double>::pi * cutoff);
	return 1.0f / (1.0f + tau / te);
}


Array<double> OneEuroFilter::filter(Array<double> oldValues, Array<double> newValues, double deltaTime)
{
	freq = 1.0 / deltaTime;

	Array<double> result;

	for (int i = 0; i < dimensions; i++)
	{
		double deltaP = (newValues[i] - oldValues[i]) * freq;

		double edvalue = dx[i]->filterWithAlpha(deltaP, alpha(derivativeCutOff));
		double cutoff = minCutOff + beta * fabs(edvalue);
		result.add(x[i]->filterWithAlpha(newValues[i], alpha(cutoff)));
	}

	return result;
}


var OneEuroFilter::filter(var oldPos, var newPos, double deltaTime)
{
	Array<double> oldValues;
	Array<double> newValues;

	if (newPos.isArray())
	{
		for (int i = 0; i < newPos.size(); i++)
		{
			jassert(!isnan((double)newPos[i]));
			oldValues.add(oldPos[i]);
			newValues.add(newPos[i]);
		}
	}
	else
	{
		oldValues.add(oldPos);
		newValues.add(newPos);
	}

	Array<double> p = filter(oldValues, newValues, deltaTime);

	var result;
	for (int i = 0; i < p.size(); i++) result.append(p[i]);

	return result;
}


OneEuroFloatFilter::OneEuroFloatFilter() :
	OneEuroFilter(1)
{
}

OneEuroFloatFilter::~OneEuroFloatFilter()
{
}

double OneEuroFloatFilter::filter(double oldValue, double newValue, double deltaTime)
{
	Array<double> p = OneEuroFilter::filter(Array<double>{ oldValue }, Array<double>{ newValue }, deltaTime);
	return p[0];
}