/*
  ==============================================================================

    ConversionFilters.cpp
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#include "ConversionFilters.h"



ConversionFilter::ConversionFilter(const String &name, var params, StringRef outTypeString) :
	MappingFilter(name, params)
{
	autoSetRange = false;
	forceOutParameterType = outTypeString;
	retargetComponent = filterParams.addEnumParameter("Extract Component", "This is the component of the source parameter to extract");
	retargetComponent->setCustomShortName("retargetComponent");
}

ConversionFilter::~ConversionFilter()
{
}

Parameter * ConversionFilter::setupParameterInternal(Parameter * source)
{
	Parameter * p = MappingFilter::setupParameterInternal(source);
	
	retargetComponent->clearOptions();

	if (p->isComplex() == sourceParam->isComplex())
	{
		transferType = DIRECT;
		retargetComponent->setEnabled(false);
	}
	else
	{
		retargetComponent->setEnabled(true);
		transferType = p->isComplex() ? TARGET : EXTRACT;
		retargetComponent->setNiceName(transferType == TARGET ? "Target Component" : "Extract Component");
		Parameter * retargetP = transferType == TARGET ? p : sourceParam.get();
		StringArray valueNames = retargetP->getValuesNames();
		for (int i = 0; i < valueNames.size(); i++)
		{
			retargetComponent->addOption(valueNames[i], i);
		}
	}

	return p;
}

void ConversionFilter::processInternal()
{
	if (filteredParameter == nullptr || sourceParam == nullptr) return;

	switch (transferType)
	{
	case DIRECT:
	{
		if (!sourceParam->isComplex()) filteredParameter->setValue(convertValue(sourceParam->getValue()));
		else
		{
			var val = var();
			var sourceVal = sourceParam->getValue();
			for (int i = 0; i < sourceParam->value.size() && i < filteredParameter->value.size(); i++) val.append(convertValue(sourceVal[i]));
			while (val.size() < filteredParameter->value.size()) val.append(0);
			filteredParameter->setValue(val);
		}
	}
	break;

	case EXTRACT: 
	{
		filteredParameter->setValue(convertValue(sourceParam->value[(int)retargetComponent->getValueData()]));
	}
	break;

	case TARGET:
	{
		var val = var();
		for (int i = 0; i < filteredParameter->value.size(); i++) val.append(0);
		val[(int)retargetComponent->getValueData()] = convertValue(sourceParam->getValue());
		filteredParameter->setValue(val);
	}
	break;
	}
}



ToFloatFilter::ToFloatFilter(var params) :
	ConversionFilter(getTypeString(), params, FloatParameter::getTypeStringStatic())
{
}

var ToFloatFilter::convertValue(var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}


ToIntFilter::ToIntFilter(var params) :
	ConversionFilter(getTypeString(), params, IntParameter::getTypeStringStatic())
{
	modeParam = addEnumParameter("Mode", "Conversion mode");
	modeParam->addOption("Round", ROUND)->addOption("Floor", FLOOR)->addOption("Ceil", CEIL);
}

var ToIntFilter::convertValue(var sourceValue)
{
	Mode m = modeParam->getValueDataAsEnum<Mode>();
	switch(m)
	{
	case ROUND: return roundf(sourceParam->floatValue());
	case FLOOR: return floorf(sourceParam->floatValue()); 
	case CEIL:  return ceilf(sourceParam->floatValue());
	}

	return 0;
}

ToStringFilter::ToStringFilter(var params) :
	ConversionFilter(getTypeString(), params, StringParameter::getTypeStringStatic())
{
	//numLeadingZeros = filterParams.addIntParameter("Leadings zeros", "Number of leading zero to add in front of number if necessary", 0, 0, 10);
	numDecimals = filterParams.addIntParameter("Number of Decimals", "Maximum number of decimals", 3, 0, 26);
}

var ToStringFilter::convertValue(var sourceValue)
{
	if (sourceValue.isString()) return sourceValue;
	return String((float)sourceValue, numDecimals->intValue());
}

ToPoint2DFilter::ToPoint2DFilter(var params) :
	ConversionFilter(getTypeString(), params, Point2DParameter::getTypeStringStatic())
{
}

var ToPoint2DFilter::convertValue(var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}

ToPoint3DFilter::ToPoint3DFilter(var params) :
	ConversionFilter(getTypeString(), params, Point3DParameter::getTypeStringStatic())
{
}

var ToPoint3DFilter::convertValue(var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}
