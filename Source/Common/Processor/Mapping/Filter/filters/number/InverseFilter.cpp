/*
  ==============================================================================

    InverseFilter.cpp
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

InverseFilter::InverseFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true)
{
	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::BOOL);
}

InverseFilter::~InverseFilter()
{
}

MappingFilter::ProcessResult InverseFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!source->hasRange())
	{
		out->setValue(source->getValue());
		return CHANGED;
	}

	if (source->type == source->BOOL) out->setValue(!source->boolValue());
	else out->setValue(jmap<float>(source->getNormalizedValue(), source->maximumValue, source->minimumValue));
	
	return CHANGED;
}
