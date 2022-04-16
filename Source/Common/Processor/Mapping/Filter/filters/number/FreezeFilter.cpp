/*
  ==============================================================================

	FreezeFilter.cpp
	Created: 16 Apr 2022 9:35:18pm
	Author:  bkupe

  ==============================================================================
*/


FreezeFilter::FreezeFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::POINT2D, Controllable::POINT3D);

	mode = filterParams.addEnumParameter("Mode", "The mode to use");
	mode->addOption("Hold", HOLD)->addOption("Min", MIN)->addOption("Max", MAX);

	reset = filterParams.addTrigger("Reset", "Reset the values");
}

FreezeFilter::~FreezeFilter()
{

}


void FreezeFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	prevValueMap.clear();
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);
}

Parameter* FreezeFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	var v;
	if (source->isComplex()) for (int i = 0; i < source->value.size(); i++) v.append(p->value[i]);
	else v = p->getValue();

	prevValueMap.set(source, source->getValue());
	return p;
}

MappingFilter::ProcessResult FreezeFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	prevValueMap.set(out, newVal);

	FreezeMode m = mode->getValueDataAsEnum<FreezeMode>();

	var v;

	if (!source->isComplex())
	{
		switch (m)
		{
		case HOLD: v = oldVal; break;
		case MIN: v = jmin<float>(oldVal, newVal); break;
		case MAX: v = jmax<float>(oldVal, newVal); break;
		}
	}
	else
	{
		for (int i = 0; i < out->value.size(); i++)
		{
			switch (m)
			{
			case HOLD: v.append(oldVal[i]); break;
			case MIN: v.append(jmin<float>(oldVal, newVal)); break;
			case MAX: v.append(jmax<float>(oldVal, newVal)); break;
			}
		}
	}

	out->setValue(v);

	return CHANGED;
}

void FreezeFilter::onContainerTriggerTriggered(Trigger* t)
{
	MappingFilter::onContainerTriggerTriggered(t);
	if (t == reset)
	{
		HashMap<Parameter*, var>::Iterator it(prevValueMap);
		while (it.next()) it.getKey()->setValue(it.getValue());
	}
}
