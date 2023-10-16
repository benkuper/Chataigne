/*
  ==============================================================================

	DelayFilter.cpp
	Created: 12 Oct 2021 10:17:44am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"


DelayFilter::DelayFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true)
{
	delay = filterParams.addFloatParameter("Delay", "Delay in seconds", 1, 0);
	delay->defaultUI = FloatParameter::TIME;
	processOnSameValue = true;
}

DelayFilter::~DelayFilter()
{

}

void DelayFilter::multiplexCountChanged()
{
	MappingFilter::multiplexCountChanged();
	paramTimeValueMap.clear();
}

void DelayFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	if (!rangeOnly && multiplexIndex == 0) paramTimeValueMap.clear(); // only multiplex 0 should clear the map when setting up sources
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);
}

Parameter* DelayFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	if (!rangeOnly)
	{
		var tmpVal = var(source->getValue()); //shoud maybe copy the values or is it enough ?
		paramTimeValueMap.set(source, Array<TimeValue>({ Time::getMillisecondCounterHiRes() / 1000.0, tmpVal }));
	}

	return MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
}

MappingFilter::ProcessResult  DelayFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!paramTimeValueMap.contains(source)) return UNCHANGED;
	Array<TimeValue>* values = &paramTimeValueMap.getReference(source);

	double t = Time::getMillisecondCounterHiRes() / 1000.0;

	var val = source->getValue();
	DBG(multiplexIndex << " : " << val.toString() << " <> " << out->stringValue());

	if (values->size() > 0)
	{
		TimeValue tv = values->getUnchecked(values->size() - 1);
		if (tv.val != val) values->add({ t + delay->floatValue(), val });
	}
	else if (!out->checkValueIsTheSame(val, out->getValue()))
	{
		values->add({ t + (float)filterParams.getLinkedValue(delay, multiplexIndex), val });
	}


	bool hasChanged = false;
	while (values->size() > 0)
	{
		TimeValue tv = values->getUnchecked(0);
		if (tv.time < t)
		{
			val = tv.val;
			values->remove(0);
			hasChanged = true;
		}
		else
		{
			break;
		}
	}

	if (!hasChanged) return UNCHANGED;

	out->setValue(val);
	return CHANGED;
}

void DelayFilter::filterParamChanged(Parameter* p)
{
	if (p == delay)
	{
		HashMap<WeakReference<Parameter>, Array<TimeValue>>::Iterator it(paramTimeValueMap);
		while (it.next())
		{
			WeakReference<Parameter> p = it.getKey();
			if (p.wasObjectDeleted()) continue;

			Array<TimeValue>* a = &paramTimeValueMap.getReference(p);
			a->clear();
			a->add({ Time::getMillisecondCounterHiRes() / 1000.0, it.getKey()->getValue() });
		}
	}
}