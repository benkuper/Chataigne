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
	addCompareOption("Change", changeId);
}

BoolComparator::~BoolComparator()
{
}

bool BoolComparator::compareInternal(Parameter * sourceParam, int multiplexIndex)
{
	return compareValue(sourceParam->getValue(), multiplexIndex);
}

bool BoolComparator::compareValue(const var& sourceValue, int multiplexIndex)
{
	const ScopedLock lock(compareLock);
	if (reference == nullptr || currentFunctionId.isNull()) return false;
	if (currentFunctionId == changeId) return true;
	bool value = isMultiplexed() ? (bool)refLink->getLinkedValue(multiplexIndex) : reference->boolValue();
	if (currentFunctionId == equalsId) return (bool)sourceValue == value;
	if (currentFunctionId == differentId) return (bool)sourceValue != value;
	return false;
}