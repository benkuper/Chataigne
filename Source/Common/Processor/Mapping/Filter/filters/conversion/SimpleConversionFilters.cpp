#include "SimpleConversionFilters.h"
/*
  ==============================================================================

	SimpleConversionFilters.cpp
	Created: 17 Apr 2018 10:25:04am
	Author:  Ben

  ==============================================================================
*/

SimpleConversionFilter::SimpleConversionFilter(const String& name, var params, StringRef outTypeString, Multiplex* multiplex) :
	MappingFilter(name, params, multiplex),
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

Parameter* SimpleConversionFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	if (rangeOnly)
	{
		int index = sourceParams[multiplexIndex].indexOf(source);
		if (index >= 0)
		{
			Parameter* fp = (*filteredParameters[multiplexIndex])[index];
			if (fp->isComplex() == source->isComplex())
			{
				if (source->hasRange() && fp->canHaveRange) fp->setRange(source->minimumValue, source->maximumValue);
			}
		}
	}

	Parameter* p = (Parameter*)ControllableFactory::getInstance()->createControllable(outTypeString);
	p->setNiceName(source->niceName);

	if (multiplexIndex != 0) return p; //only setup for 1st if multiplex multiplex

	retargetComponent->clearOptions();

	if (p->isComplex() == source->isComplex())
	{
		if (source->hasRange() && p->canHaveRange) p->setRange(source->minimumValue, source->maximumValue);
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
		Parameter* retargetP = transferType == TARGET ? p : source;
		StringArray valueNames = retargetP->getValuesNames();
		for (int i = 0; i < valueNames.size(); ++i)
		{
			retargetComponent->addOption(valueNames[i], i, true);
		}

		addExtraRetargetOptions();


		retargetComponent->setValueWithData(ghostOptions.getProperty("retarget", valueNames[0]));
	}

	return p;
}


MappingFilter::ProcessResult SimpleConversionFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	switch (transferType)
	{
	case DIRECT:
	{
		if (!source->isComplex()) out->setValue(convertValue(source, source->getValue(), multiplexIndex));
		else
		{
			var val = var();
			var sourceVal = source->getValue();
			for (int i = 0; i < source->value.size() && i < out->value.size(); ++i) val.append(convertValue(source, sourceVal[i], multiplexIndex));
			while (val.size() < out->value.size()) val.append(0);
			out->setValue(val);
		}
	}
	break;

	case EXTRACT:
	{
		int index = (int)retargetComponent->getValueData();
		if (index < source->value.size()) out->setValue(convertValue(source, source->value[index], multiplexIndex));
	}
	break;

	case TARGET:
	{
		var val = var();
		for (int i = 0; i < out->value.size(); ++i) val.append(0);
		int index = (int)retargetComponent->getValueData();
		if (index < val.size())
		{
			val[index] = convertValue(source, source->getValue(), multiplexIndex);
			out->setValue(val);
		}
	}
	break;

	default:
		return UNCHANGED;
	}

	return CHANGED;
}

var SimpleConversionFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();

	ghostOptions = var(new DynamicObject());
	ghostOptions.getDynamicObject()->setProperty("retarget", retargetComponent->getValueData());

	data.getDynamicObject()->setProperty("ghostOptions", ghostOptions);
	return data;
}

void SimpleConversionFilter::loadJSONDataItemInternal(var data)
{
	MappingFilter::loadJSONDataItemInternal(data);
	ghostOptions = data.getProperty("ghostOptions", var());
}

ToFloatFilter::ToFloatFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, FloatParameter::getTypeStringStatic(), multiplex)
{
}

Parameter* ToFloatFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = SimpleConversionFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);

	if (source->hasRange())
	{
		if (source->minimumValue.isArray())
		{
			int index = (int)retargetComponent->getValueData();
			p->setRange(source->minimumValue[index], source->maximumValue[index]);
		}
		else
		{
			p->setRange(source->minimumValue, source->maximumValue);
		}
	}

	return p;
}

var ToFloatFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}


ToIntFilter::ToIntFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, IntParameter::getTypeStringStatic(), multiplex)
{
	modeParam = filterParams.addEnumParameter("Mode", "Conversion mode");
	modeParam->addOption("Round", ROUND)->addOption("Floor", FLOOR)->addOption("Ceil", CEIL);
}

var ToIntFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	Mode m = modeParam->getValueDataAsEnum<Mode>();

	if (source->type == Parameter::ENUM)
	{
		EnumParameter* ep = (EnumParameter*)source;
		if (ep->getValueData().isInt() || ep->getValueData().isDouble()) return (int)ep->getValueData();
		else return ep->getIndexForKey(ep->getValueKey());
	}
	else
	{
		switch (m)
		{
		case ROUND: return roundf(source->floatValue());
		case FLOOR: return floorf(source->floatValue());
		case CEIL:  return ceilf(source->floatValue());
		}

	}

	return 0;
}

ToStringFilter::ToStringFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, StringParameter::getTypeStringStatic(), multiplex),
	format(nullptr),
	numDecimals(nullptr),
	fixedLeading(nullptr),
	prefix(nullptr),
	suffix(nullptr),
	enumConvertMode(nullptr)
{
	format = filterParams.addEnumParameter("Format", "The format of the string");
	format->addOption("Number", NUMBER)->addOption("Time", TIME);
	numDecimals = filterParams.addIntParameter("Number of Decimals", "Maximum number of decimals", 3, 0, 26);
	fixedLeading = filterParams.addIntParameter("Fixed Leading", "If enabled, this will force the output to be with a certain mount of digits before the decimals", 3, 0, 100, false);
	fixedLeading->canBeDisabledByUser = true;

	prefix = filterParams.addStringParameter("Prefix", "Something prepended to the result", "");
	suffix = filterParams.addStringParameter("Suffix", "Something appended  to the result", "");
}


void ToStringFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	SimpleConversionFilter::setupParametersInternal(multiplexIndex, rangeOnly);
	if (multiplexIndex != 0 || rangeOnly) return;

	Parameter* p = sourceParams[multiplexIndex].size() > 0 ? sourceParams[multiplexIndex][0] : nullptr;
	if (p == nullptr) return;
	
	if (p->type == Parameter::FLOAT || p->type == Parameter::INT)
	{
		if (enumConvertMode != nullptr)
		{
			filterParams.removeControllable(enumConvertMode);
			enumConvertMode = nullptr;
		}
	}
	else
	{
		if (enumConvertMode == nullptr)
		{
			enumConvertMode = filterParams.addEnumParameter("Convert Mode", "What to convert in the enum");
			enumConvertMode->addOption("Key", KEY)->addOption("Value", VALUE);
		}
	}
}


var ToStringFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	String result = filterParams.getLinkedValue(prefix, multiplexIndex).toString();

	var sv = sourceValue;

	if (source->type == Parameter::ENUM)
	{
		EnumParameter* ep = (EnumParameter*)source;
		ConvertMode m = enumConvertMode != nullptr ? enumConvertMode->getValueDataAsEnum<ConvertMode>() : KEY;
		if (m == KEY) sv = ep->getValueKey();
		else sv = ep->getValueData();
	}

	if (!sv.isArray())
	{
		if (sv.isString()) result += sv.toString();
		else
		{
			if (sv.isArray())
			{
				result = "[";
				for (int i = 0; i < sv.size(); i++) result += String(i >= 0 ? "," : "") + sv[i].toString();
				result += "]";
			}
			else if (sv.isDouble() || sv.isInt() || sv.isInt64())
			{
				Format f = format->getValueDataAsEnum<Format>();
				switch (f)
				{
				case NUMBER:
					result += String((float)sv, (int)filterParams.getLinkedValue(numDecimals, multiplexIndex));
					if (fixedLeading->enabled)
					{
						StringArray s;
						s.addTokens(result, ".", "");
						String first = String::repeatedString("0", (int)filterParams.getLinkedValue(fixedLeading, multiplexIndex) - s[0].length()) + s[0];
						result = first + (s.size() > 1 ? "." + s[1] : "");
					}
					break;

				case TIME:
					result += StringUtil::valueToTimeString((float)sv);
					break;
				}
			}
		}
	}

	result += filterParams.getLinkedValue(suffix, multiplexIndex).toString();
	return result;
}

void ToStringFilter::filterParamChanged(Parameter* p)
{
	numDecimals->hideInEditor = format->getValueDataAsEnum<Format>() != NUMBER;
	SimpleConversionFilter::filterParamChanged(p);
}

ToPoint2DFilter::ToPoint2DFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, Point2DParameter::getTypeStringStatic(), multiplex)
{
}

var ToPoint2DFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}

ToPoint3DFilter::ToPoint3DFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, Point3DParameter::getTypeStringStatic(), multiplex)
{
}

var ToPoint3DFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	if (sourceValue.isString()) return sourceValue.toString().getFloatValue();
	return (float)sourceValue;
}

ToColorFilter::ToColorFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, ColorParameter::getTypeStringStatic(), multiplex),
	baseColor(nullptr)
{
}

ToColorFilter::~ToColorFilter()
{
}

var ToColorFilter::getJSONData()
{
	var data = SimpleConversionFilter::getJSONData();
	if (baseColor != nullptr) data.getProperty("ghostOptions", var()).getDynamicObject()->setProperty("color", baseColor->getValue());
	return data;
}

void ToColorFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	SimpleConversionFilter::setupParametersInternal(multiplexIndex, rangeOnly);
	if (multiplexIndex != 0 || rangeOnly) return;

	Parameter* p = sourceParams[multiplexIndex].size() > 0 ? sourceParams[multiplexIndex][0] : nullptr;
	if (p == nullptr) return;

	//String oldKey = "";
	//if (retargetComponent != nullptr) oldKey = retargetComponent->getValueKey();

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

	

	if (ghostOptions.isObject())
	{
		if (ghostOptions.hasProperty("color")) baseColor->setValue(ghostOptions.getDynamicObject()->getProperty("color"));
		ghostOptions = var();
	}
}

void ToColorFilter::addExtraRetargetOptions()
{
	switch (transferType)
	{

	case TARGET:
		retargetComponent->addOption("Hue", HUE)->addOption("Saturation", SAT)->addOption("Brightness", VAL);
		//if (oldKey.isNotEmpty()) retargetComponent->setValueWithKey(oldKey);
	//	else retargetComponent->setValueWithData(ghostOptions.getProperty("retarget", HUE));

		if (baseColor != nullptr) baseColor->hideInEditor = false;
		break;

	default:
		if (baseColor != nullptr) baseColor->hideInEditor = true;
		break;

	}
}

MappingFilter::ProcessResult ToColorFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (source->value.isArray())
	{
		if (source->value.size() >= 3)
		{
			Colour c = Colour::fromFloatRGBA((float)source->value[0], (float)source->value[1], (float)source->value[2], source->value.size() > 3 ? (float)source->value[3] : 1.0f);
			((ColorParameter*)out)->setColor(c);
			return CHANGED;
		}

		return UNCHANGED; //don't process
	}

	//single value
	switch (transferType)
	{

	case TARGET:
	{
		int comp = retargetComponent->getValueData();

		var baseColorVal = filterParams.getLinkedValue(baseColor, multiplexIndex);
		Colour mBaseColor = Colour::fromFloatRGBA((float)baseColorVal[0], (float)baseColorVal[1], (float)baseColorVal[2], baseColorVal.size() > 3 ? (float)baseColorVal[3] : 1.0f);

		switch (comp)
		{
		case HUE:
		{
			Colour c = mBaseColor.withHue(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		case SAT:
		{
			Colour c = mBaseColor.withSaturation(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		case VAL:
		{
			Colour c = mBaseColor.withBrightness(source->value);
			((ColorParameter*)out)->setColor(c);
		}
		break;

		default:
		{
			var val = var();
			for (int i = 0; i < out->value.size() && i < baseColorVal.size(); ++i) val.append(baseColorVal[i]); //force alpha to 1
			val[(int)retargetComponent->getValueData()] = convertValue(source, source->getValue(), multiplexIndex);
			out->setValue(val);
		}
		break;
		}

	}
	break;

	default:
		return SimpleConversionFilter::processSingleParameterInternal(source, out, multiplexIndex);
		break;
	}

	return CHANGED;
}


ToBooleanFilter::ToBooleanFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, BoolParameter::getTypeStringStatic(), multiplex)
{
	toggleMode = filterParams.addBoolParameter("Toggle Mode", "If checked, this will act as a toggle, and its value will be inverted when input value is 1", false);
}

MappingFilter::ProcessResult ToBooleanFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	bool val = (source->value.isString() ? source->value.toString().getFloatValue() : source->floatValue()) >= 1;

	bool tMode = (bool)(int)filterParams.getLinkedValue(toggleMode, multiplexIndex);
	if (tMode)
	{
		if (val) out->setValue(!out->boolValue());
	}
	else out->setValue(val);

	return CHANGED;
}
