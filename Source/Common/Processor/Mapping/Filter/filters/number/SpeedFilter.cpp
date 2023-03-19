/*
  ==============================================================================

	SpeedFilter.cpp
	Created: 8 Dec 2020 11:36:15am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

SpeedFilter::SpeedFilter(var params, Multiplex* multiplex) :
	TimeFilter(getTypeString(), params, multiplex)
{
	autoSetRange = false;
	processOnSameValue = true;
	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::POINT2D, Controllable::POINT3D);

	smoothing = filterParams.addIntParameter("Smoothing", "Number of values to use for smoothing", 10, 1);

}

SpeedFilter::~SpeedFilter()
{

}


void SpeedFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	prevValueMap.clear();
	speedMap.clear();
	TimeFilter::setupParametersInternal(multiplexIndex, rangeOnly);
}

Parameter* SpeedFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;

	if (!rangeOnly && source->type == Controllable::INT)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = TimeFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
		p->clearRange();
	}

	var s;
	var v;
	if (source->isComplex()) for (int i = 0; i < source->value.size(); i++) v.append(0);
	else v = 0;

	for (int i = 0; i < smoothing->intValue(); i++) s.append(v);
	speedMap.set(source, s);

	prevValueMap.set(source, source->getValue());
	return p;
}

MappingFilter::ProcessResult SpeedFilter::processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime)
{
	var oldVal = prevValueMap[source];
	var newVal = source->getValue();

	prevValueMap.set(source, newVal);

	var speed;

	if (deltaTime == 0) return UNCHANGED;

	if (!source->isComplex())
	{
		speed = ((double)newVal - (double)oldVal) / deltaTime;
		if (isnan((float)speed) || isinf((float)speed)) speed = 0;
	}
	else
	{
		for (int i = 0; i < out->value.size(); i++)
		{
			double s = ((double)newVal[i] - (double)oldVal[i]) / deltaTime;
			if (isinf(s)) speed.append(oldVal[i]);
			speed.append(s);
		}
	}

	var sMap = speedMap[source];
	sMap.remove(0);
	sMap.append(speed);

	int smoothCount = smoothing->intValue();
	var smoothSpeed;
	if (speed.isArray())
	{
		for (int j = 0; j < speed.size(); j++)
		{
			float s = 0;
			for (int i = 0; i < smoothCount && i < sMap.size(); i++) s += (double)sMap[i][j] / smoothCount;
			smoothSpeed.append(s);
		}
	}
	else
	{
		smoothSpeed = 0;
		for (int i = 0; i < smoothCount && i < sMap.size(); i++) smoothSpeed = (double)smoothSpeed + (double)sMap[i] / smoothCount;
	}

	out->setValue(smoothSpeed);
	speedMap.set(source, sMap);

	return CHANGED;
}

void SpeedFilter::filterParamChanged(Parameter* p)
{
	TimeFilter::filterParamChanged(p);
	if (p == smoothing)
	{
		if (!isCurrentlyLoadingData) setupParametersInternal(-1, false);

		mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
		filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}
}