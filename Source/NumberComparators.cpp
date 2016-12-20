/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "NumberComparators.h"

FloatComparator::FloatComparator(Controllable * c) :
	ParameterComparator(c),
	floatParam((FloatParameter *)c)
{
	addCompareOption("=", equalsId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);


	floatRef = addFloatParameter("Reference", "Comparison Reference to check against source value", floatParam->defaultValue,floatParam->minimumValue,floatParam->maximumValue);
	reference = floatRef;
}

FloatComparator::~FloatComparator()
{
}

void FloatComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(floatParam->floatValue() == floatRef->floatValue());
	else if (currentFunctionId == greaterId)		setValid(floatParam->floatValue() > floatRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(floatParam->floatValue() < floatRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(floatParam->floatValue() >= floatRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(floatParam->floatValue() <= floatRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}


IntComparator::IntComparator(Controllable * c) :
	ParameterComparator(c),
	intParam((IntParameter *)c)
{
	addCompareOption("=", equalsId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("~", inRangeId);

	intRef = addIntParameter("Reference", "Comparison Reference to check against source value", intParam->defaultValue,intParam->minimumValue,intParam->maximumValue);
	reference = intRef;
}

IntComparator::~IntComparator()
{
}

void IntComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(intParam->floatValue() == intRef->floatValue());
	else if (currentFunctionId == greaterId)				setValid(intParam->floatValue() > intRef->floatValue());
	else if (currentFunctionId == lessId)			setValid(intParam->floatValue() < intRef->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(intParam->floatValue() >= intRef->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(intParam->floatValue() <= intRef->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}

