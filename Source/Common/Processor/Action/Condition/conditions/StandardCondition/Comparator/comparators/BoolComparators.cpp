/*
  ==============================================================================

    BoolComparators.cpp
    Created: 2 Nov 2016 8:58:50pm
    Author:  bkupe

  ==============================================================================
*/

BoolComparator::BoolComparator(Parameter * sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	setReferenceParam(new BoolParameter("Reference", "Comparison Reference to check against source value", sourceParam->boolValue()));
	reference->setValue(sourceParam->boolValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption("Change", Identifier());
}

BoolComparator::~BoolComparator()
{
}

bool BoolComparator::compareInternal(Parameter * sourceParam, int multiplexIndex)
{
	if (currentFunctionId.isNull()) return true;
	bool value = isMultiplexed() ? (bool)refLink->getLinkedValue(multiplexIndex) : reference->boolValue();
	if (currentFunctionId == equalsId) return sourceParam->boolValue() == value;
	if (currentFunctionId == differentId) return sourceParam->boolValue() != reference->boolValue();
	return false;
}