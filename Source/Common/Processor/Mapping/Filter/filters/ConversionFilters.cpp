/*
  ==============================================================================

    ConversionFilters.cpp
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#include "ConversionFilters.h"

ToIntFilter::ToIntFilter(var params)
{
	forceOutParameterType = IntParameter::getTypeStringStatic();
	modeParam = addEnumParameter("Mode", "Conversion mode");
	modeParam->addOption("Round", ROUND)->addOption("Floor", FLOOR)->addOption("Ceil", CEIL);
}

ToIntFilter::~ToIntFilter()
{
}

void ToIntFilter::processInternal()
{
	if (sourceParam->type == Parameter::INT) filteredParameter->setValue(sourceParam->value);
	else
	{
		Mode m = modeParam->getValueDataAsEnum<Mode>();
		if (m == ROUND) filteredParameter->setValue(roundf(sourceParam->floatValue())); 
		else if (m == FLOOR) filteredParameter->setValue(floorf(sourceParam->floatValue()));
		else if (m == CEIL) filteredParameter->setValue(ceilf(sourceParam->floatValue()));
	}
}
