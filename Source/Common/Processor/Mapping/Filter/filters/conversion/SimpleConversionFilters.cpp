/*
  ==============================================================================

    SimpleConversionFilters.cpp
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#include "SimpleConversionFilters.h"

SimpleConversionFilter::SimpleConversionFilter(const String &name, var params, StringRef outTypeString) :
	MappingFilter(name, params),
	retargetComponent(nullptr),
	outTypeString(outTypeString)
{
	autoSetRange = false;
	retargetComponent = filterParams.addEnumParameter("Extract Component", "This is the component of the source parameter to extract");
	retargetComponent->setCustomShortName("retargetComponent");
	retargetComponent->forceSaveValue = true;
}

SimpleConversionFilter::~SimpleConversionFilter()
{
}

Parameter* SimpleConversionFilter::setupSingleParameterInternal(Parameter* source)
{
	Parameter* p = (Parameter *)ControllableFactory::getInstance()->createControllable(outTypeString);
	p->setNiceName(source->niceName);

	retargetComponent->clearOptions();

	if (p->isComplex() == source->isComplex())
	{
		transferType = DIRECT;
		retargetComponent->setEnabled(false);
		retargetComponent->hideInEditor = true;
	}
	else
	{
		retargetComponent->setEnabled(true);
		retargetComponent->hideInEditor = false;
		transferType = p->isComplex() ? TARGET : EXTRACT;
		retargetComponent->setNiceName(transferType == TARGET ? "Target Component" : "Extract Component");
		Parameter * retargetP = transferType == TARGET ? p : source;
		StringArray valueNames = retargetP->getValuesNames();
		for (int i = 0; i < valueNames.size(); ++i)
		{
			retargetComponent->addOption(valueNames[i], i, false);
		}

	}

	return p;
}


void SimpleConversionFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	switch (transferType)
	{
	case DIRECT:
	{
		if (!source->isComplex()) out->setValue(convertValue(source, source->getValue()));
		else
		{
			var val = var();
			var sourceVal = source->getValue();
			for (int i = 0; i < source->value.size() && i < out->value.size(); ++i) val.append(convertValue(source, sourceVal[i]));
			while (val.size() < out->value.size()) val.append(0);
			out->setValue(val);
		}
	}
	break;

	case EXTRACT: 
	{
		out->setValue(convertValue(source, source->value[(int)retargetComponent->getValueData()]));
	}
	break;

	case TARGET:
	{
		var val = var();
		for (int i = 0; i < out->value.size(); ++i) val.append(0);
		val[(int)retargetComponent->getValueData()] = convertValue(source, source->getValue());
		out->setValue(val);
	}
	break;
	}
}



ToFloatFilter::ToFloatFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, FloatParameter::getTypeStringStatic())
{
}

var ToFloatFilter::convertValue(Parameter * source, var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}


ToIntFilter::ToIntFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, IntParameter::getTypeStringStatic())
{
	modeParam = addEnumParameter("Mode", "Conversion mode");
	modeParam->addOption("Round", ROUND)->addOption("Floor", FLOOR)->addOption("Ceil", CEIL);
}

var ToIntFilter::convertValue(Parameter * source, var sourceValue)
{
	Mode m = modeParam->getValueDataAsEnum<Mode>();
	switch(m)
	{
	case ROUND: return roundf(source->floatValue());
	case FLOOR: return floorf(source->floatValue()); 
	case CEIL:  return ceilf(source->floatValue());
	}

	return 0;
}

ToStringFilter::ToStringFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, StringParameter::getTypeStringStatic())
{
	format = filterParams.addEnumParameter("Format", "The format of the string");
	format->addOption("Number", NUMBER)->addOption("Time", TIME);
	numDecimals = filterParams.addIntParameter("Number of Decimals", "Maximum number of decimals", 3, 0, 26);
	prefix = filterParams.addStringParameter("Prefix", "Something prepended to the result", "");
	suffix = filterParams.addStringParameter("Suffix", "Something appended  to the result", "");
}

var ToStringFilter::convertValue(Parameter * source, var sourceValue)
{
	String result = prefix->stringValue();

	if (!sourceValue.isArray())
	{
		if (sourceValue.isString()) result += sourceValue.toString();
		else
		{
			Format f = format->getValueDataAsEnum<Format>();
			switch (f)
			{
			case NUMBER:
				result += String((float)sourceValue, numDecimals->intValue());
				break;

			case TIME:
				result += StringUtil::valueToTimeString((float)sourceValue);
				break;
			}
		}
	}
	
	result += suffix->stringValue();
	return result;
}

void ToStringFilter::filterParamChanged(Parameter * p)
{
	numDecimals->hideInEditor = format->getValueDataAsEnum<Format>() != NUMBER;
	SimpleConversionFilter::filterParamChanged(p);
}

ToPoint2DFilter::ToPoint2DFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, Point2DParameter::getTypeStringStatic())
{
}

var ToPoint2DFilter::convertValue(Parameter * source, var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}

ToPoint3DFilter::ToPoint3DFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, Point3DParameter::getTypeStringStatic())
{
}

var ToPoint3DFilter::convertValue(Parameter * source, var sourceValue)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}

ToColorFilter::ToColorFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, ColorParameter::getTypeStringStatic()),
	baseColor(nullptr)
{
}

ToColorFilter::~ToColorFilter()
{
}

Parameter* ToColorFilter::setupSingleParameterInternal(Parameter* sourceParam)
{
	Parameter* p = SimpleConversionFilter::setupSingleParameterInternal(sourceParam);
	
	if (transferType != TARGET)
	{
		if (baseColor != nullptr)
		{
			filterParams.removeControllable(baseColor);
			baseColor = nullptr;
		}
		
	}
	else if (baseColor == nullptr)
	{
		baseColor = filterParams.addColorParameter("Base Color", "Color to use to convert", Colours::red);
	}

	switch(transferType)
	{

	case TARGET:
		retargetComponent->addOption("Hue", HUE)->addOption("Saturation", SAT)->addOption("Brightness", VAL);
		retargetComponent->setValueWithData(HUE);
		if(baseColor != nullptr) baseColor->hideInEditor = false;
		break;
            
    default:
		if(baseColor != nullptr) baseColor->hideInEditor = true;
        break;

	}

	return p;
}

void ToColorFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	switch (transferType)
	{

	case TARGET:
	{
		int comp = retargetComponent->getValueData();

		switch (comp)
		{
		case HUE:
		{
			Colour c = baseColor->getColor().withHue(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		case SAT:
		{
			Colour c = baseColor->getColor().withSaturation(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		case VAL:
		{
			Colour c = baseColor->getColor().withBrightness(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		default:
		{
			var val = var();
			for (int i = 0; i < out->value.size(); ++i) val.append(baseColor->value[i]); //force alpha to 1
			val[(int)retargetComponent->getValueData()] = convertValue(source, source->getValue());
			out->setValue(val);
		}
		break;
		}
		
	}
	break;

	default:
		SimpleConversionFilter::processInternal();
		break;
	}

	
}

ToBooleanFilter::ToBooleanFilter(var params) :
	SimpleConversionFilter(getTypeString(), params, BoolParameter::getTypeStringStatic())
{
	toggleMode = addBoolParameter("Toggle Mode", "If checked, this will act as a toggle, and its value will be inverted when input value is 1", false);
}

void ToBooleanFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	bool val = (source->value.isString() ?source->value.toString().getFloatValue():source->floatValue()) >= 1;
	if (toggleMode->boolValue())
	{
		if (val) out->setValue(!out->boolValue());
	}
	else out->setValue(val);
}
