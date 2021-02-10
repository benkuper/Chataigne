/*
  ==============================================================================

	DampingFilter.cpp
	Created: 8 Dec 2020 11:36:15am
	Author:  bkupe

  ==============================================================================
*/

#include "DampingFilter.h"

DampingFilter::DampingFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	processOnSameValue = true;

	force = filterParams.addFloatParameter("Force", "Attraction force", .6f, 0, 1);
	friction = filterParams.addFloatParameter("Friction", "Friction to apply to the attraction force", .2f, 0, 1);

	timeAtLastUpdate = Time::getMillisecondCounter() / 1000.0;

	filterTypeFilters.add(Controllable::BOOL, Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

DampingFilter::~DampingFilter()
{

}


void DampingFilter::setupParametersInternal(int multiplexIndex)
{
	previousSpeedsMap.clear();
	MappingFilter::setupParametersInternal(multiplexIndex);
}

Parameter* DampingFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex)
{
	Parameter* p = nullptr;
	if (source->type == Controllable::BOOL)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex);
	}

	if (!source->isComplex())
	{
		previousSpeedsMap.set(source, 0);
	}
	else
	{
		var s;
		for (int i = 0; i < source->value.size(); i++) s.append(0);
		previousSpeedsMap.set(source, s);
	}

	return p;
}

bool DampingFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!previousSpeedsMap.contains(source)) return false;

	var oldVal = out->getValue();
	var newVal = source->getValue();

	double curTime = Time::getMillisecondCounter() / 1000.0;

	if (source->checkValueIsTheSame(oldVal, newVal))
	{
		timeAtLastUpdate = curTime;
		return false;
	}

	double deltaTime = jmax<double>(curTime - timeAtLastUpdate, 0);

	float forceVal = (float)filterParams.getLinkedValue(force, multiplexIndex) * 100;
	float frictionVal = (float)filterParams.getLinkedValue(friction, multiplexIndex) * 100;

	if (!source->isComplex())
	{
		float prevSpeed = previousSpeedsMap[source];
		float acc = ((float)newVal - (float)oldVal) * forceVal;
		acc -= prevSpeed * frictionVal;
		float targetSpeed = prevSpeed + acc * deltaTime;
		if (isnan(targetSpeed)) targetSpeed = 0;

		float targetVal = (float)oldVal + targetSpeed * deltaTime;
		if (isnan(targetVal)) targetVal = newVal;
		
		if (fabsf(targetVal - (float)newVal) < precision && fabsf(acc) < precision) targetVal = newVal;
		
		out->setValue(targetVal);
		previousSpeedsMap.set(source, targetSpeed);
	}
	else
	{
		var outVal;
		var targetSpeedVal;
		for (int i = 0; i < out->value.size(); i++)
		{
			float prevSpeed = previousSpeedsMap[source][i];
			float acc = ((float)newVal[i] - (float)oldVal[i]) * forceVal;
			acc -= prevSpeed * frictionVal;
			float targetSpeed = prevSpeed + acc * deltaTime;
			if (isnan(targetSpeed)) targetSpeed = 0;

			float targetVal = (float)oldVal[i] + targetSpeed * deltaTime;
			if (isnan(targetVal)) targetVal = newVal;

			if (fabsf(targetVal - (float)newVal[i]) < precision && fabsf(acc) < precision) targetVal = newVal[i];

			outVal.append(targetVal);
			targetSpeedVal.append(targetSpeed);
		}

		out->setValue(outVal);

		previousSpeedsMap.set(source, targetSpeedVal);
	}
	

	timeAtLastUpdate = curTime;

	return true;
}