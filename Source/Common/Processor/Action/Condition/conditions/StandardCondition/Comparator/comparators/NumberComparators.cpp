/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "NumberComparators.h"

NumberComparator::NumberComparator(Parameter * sourceParam)
{
	if (FloatParameter * fp = dynamic_cast<FloatParameter *>(sourceParam))
	{
		reference = addFloatParameter("Reference", "Comparison Reference to check against source value", sourceParam->floatValue(), sourceParam->minimumValue, sourceParam->maximumValue);
		((FloatParameter *)reference)->defaultUI = ((FloatParameter *)sourceParam)->defaultUI;
	}
	else if (IntParameter* ip = dynamic_cast<IntParameter*>(sourceParam))
	{
		reference = addIntParameter("Reference", "Comparison Reference to check against source value", sourceParam->intValue(), sourceParam->minimumValue, sourceParam->maximumValue);
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

bool NumberComparator::compare(Parameter* sourceParam)
{
	if (currentFunctionId == equalsId)				return sourceParam->floatValue() == reference->floatValue();
	else if (currentFunctionId == differentId)		return sourceParam->floatValue() != reference->floatValue();
	else if (currentFunctionId == greaterId)		return sourceParam->floatValue() > reference->floatValue();
	else if (currentFunctionId == lessId)			return sourceParam->floatValue() < reference->floatValue();
	else if (currentFunctionId == greaterOrEqualId)	return sourceParam->floatValue() >= reference->floatValue();
	else if (currentFunctionId == lessOrEqualId)	return sourceParam->floatValue() <= reference->floatValue();
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

void IntComparator::compare(int iterationIndex)
{
	if (currentFunctionId == equalsId)				setValid(iterationIndex, intParams[iterationIndex]->floatValue() == intRef->floatValue());
	if (currentFunctionId == differentId)			setValid(iterationIndex, intParams[iterationIndex]->floatValue() != intRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(iterationIndex, intParams[iterationIndex]->floatValue() > intRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(iterationIndex, intParams[iterationIndex]->floatValue() < intRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(iterationIndex, intParams[iterationIndex]->floatValue() >= intRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(iterationIndex, intParams[iterationIndex]->floatValue() <= intRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(iterationIndex, false); //not implemented, need RangeParameter
}
*/