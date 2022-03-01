/*
  ==============================================================================

    EnumComparator.cpp
    Created: 19 Feb 2017 5:48:15pm
    Author:  Ben

  ==============================================================================
*/

EnumComparator::EnumComparator(Parameter * sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	EnumParameter* ep = (EnumParameter *)sourceParam;

	setReferenceParam(new EnumParameter("Reference", "Comparison Reference to check against source value"));
	enumRef = (EnumParameter*)reference;

	for (auto &ev : ep->enumValues) enumRef->addOption(ev->key, ev->value);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption("Change", Identifier());

	enumRef->setValue(ep->value, false, true, true);
}

EnumComparator::~EnumComparator()
{
}

bool EnumComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	if (currentFunctionId.isNull()) return true;
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : enumRef->getValueData();
	if (currentFunctionId == equalsId) return ((EnumParameter*)sourceParam)->getValueData() == value;
	if (currentFunctionId == differentId) return ((EnumParameter *)sourceParam)->getValueData() != value;
	return false;
}
