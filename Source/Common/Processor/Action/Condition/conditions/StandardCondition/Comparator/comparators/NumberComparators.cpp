/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

NumberComparator::NumberComparator(Parameter * sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
	if (FloatParameter * fp = dynamic_cast<FloatParameter *>(sourceParam))
	{
		setReferenceParam(new FloatParameter("Reference", "Comparison Reference to check against source value", sourceParam->floatValue(), sourceParam->minimumValue, sourceParam->maximumValue));
		((FloatParameter *)reference)->defaultUI = ((FloatParameter *)sourceParam)->defaultUI;
	}
	else if (IntParameter* ip = dynamic_cast<IntParameter*>(sourceParam))
	{
		setReferenceParam(new IntParameter("Reference", "Comparison Reference to check against source value", sourceParam->intValue(), sourceParam->minimumValue, sourceParam->maximumValue));
	}
	
	reference->setValue(sourceParam->floatValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	//addCompareOption("~", inRangeId);
}

NumberComparator::~NumberComparator()
{
}

bool NumberComparator::compare(Parameter* sourceParam, int multiplexIndex)
{
	float value = isMultiplexed() ? (float)refLink->getLinkedValue(multiplexIndex) : reference->floatValue();

	if (currentFunctionId == equalsId)				return sourceParam->floatValue() == value;
	else if (currentFunctionId == differentId)		return sourceParam->floatValue() != value;
	else if (currentFunctionId == greaterId)		return sourceParam->floatValue() > value;
	else if (currentFunctionId == lessId)			return sourceParam->floatValue() < value;
	else if (currentFunctionId == greaterOrEqualId)	return sourceParam->floatValue() >= value;
	else if (currentFunctionId == lessOrEqualId)	return sourceParam->floatValue() <= value;
	else if (currentFunctionId == inRangeId)		return false; //not implemented, need RangeParameter
	return false;
}

/*
IntComparator::IntComparator(Array<WeakReference<Controllable>> sources) :
	ParameterComparator(sources)
{
	intParams.addArray(sources);

	intRef = addIntParameter("Reference", "Comparison Reference to check against source value", intParams[0]->defaultValue, intParams[0]->minimumValue, intParams[0]->maximumValue);
	reference = intRef;
	intRef->setValue(intRef->floatValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);
}

IntComparator::~IntComparator()
{
}

void IntComparator::compare(int multiplexIndex)
{
	if (currentFunctionId == equalsId)				setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() == intRef->floatValue());
	if (currentFunctionId == differentId)			setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() != intRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() > intRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() < intRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() >= intRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(multiplexIndex, intParams[multiplexIndex]->floatValue() <= intRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(multiplexIndex, false); //not implemented, need RangeParameter
}
*/