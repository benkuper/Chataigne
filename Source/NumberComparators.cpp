/*
  ==============================================================================

    NumberComparators.cpp
    Created: 2 Nov 2016 8:57:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "NumberComparators.h"
#include "FloatComparatorUI.h"

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


	ref = addFloatParameter("Reference", "Comparison Reference to check against source value", floatParam->defaultValue,floatParam->minimumValue,floatParam->maximumValue);
}

FloatComparator::~FloatComparator()
{
}

void FloatComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(floatParam->floatValue() == ref->floatValue());
	else if (currentFunctionId == greaterId)		setValid(floatParam->floatValue() > ref->floatValue());
	else if (currentFunctionId == lessId)			setValid(floatParam->floatValue() < ref->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(floatParam->floatValue() >= ref->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(floatParam->floatValue() <= ref->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}

BaseComparatorUI * FloatComparator::createUI()
{
	return new FloatComparatorUI(this);
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

	ref = addIntParameter("Reference", "Comparison Reference to check against source value", intParam->defaultValue,intParam->minimumValue,intParam->maximumValue);
}

IntComparator::~IntComparator()
{
}

void IntComparator::compare()
{
	if (currentFunctionId == equalsId)				setValid(intParam->floatValue() == ref->floatValue());
	else if (currentFunctionId == greaterId)				setValid(intParam->floatValue() > ref->floatValue());
	else if (currentFunctionId == lessId)			setValid(intParam->floatValue() < ref->floatValue());
	else if (currentFunctionId == greaterOrEqualId)	setValid(intParam->floatValue() >= ref->floatValue());
	else if (currentFunctionId == lessOrEqualId)	setValid(intParam->floatValue() <= ref->floatValue());
	else if (currentFunctionId == inRangeId)		setValid(false); //not implemented, need RangeParameter
}
