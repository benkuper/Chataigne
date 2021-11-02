/*
  ==============================================================================

    StringComparators.cpp
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

StringComparator::StringComparator(Parameter *sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	setReferenceParam(new StringParameter("Reference", "Comparison Reference to check against source value", sourceParam->stringValue()));
	reference->setValue(sourceParam->stringValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption("Contains", containsId);
	addCompareOption("Starts with", startsWith);
	addCompareOption("Ends with", endsWidth);
}

StringComparator::~StringComparator()
{
}

bool StringComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	String value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex).toString() : reference->stringValue();

	if (currentFunctionId == equalsId)				return sourceParam->stringValue() == value;
	else if (currentFunctionId == differentId)		return sourceParam->stringValue() != value;
	else if (currentFunctionId == containsId)		return sourceParam->stringValue().contains(value);
	else if (currentFunctionId == startsWith)		return sourceParam->stringValue().startsWith(value);
	else if (currentFunctionId == endsWidth)		return sourceParam->stringValue().endsWith(value);
	return false;
}