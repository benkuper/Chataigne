/*
  ==============================================================================

	SimpleConversionFilters.cpp
	Created: 17 Apr 2018 10:25:04am
	Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

SimpleConversionFilter::SimpleConversionFilter(const String& name, var params, StringRef outTypeString, Multiplex* multiplex) :
	MappingFilter(name, params, multiplex),
	outTypeString(outTypeString),
	autoLoadDataOnSetup(true),
	useBaseValue(true),
	baseValue(nullptr)

{
	autoSetRange = false;

	retargetComponent = filterParams.addEnumParameter("Extract Component", "This is the component of the source parameter to extract");
	retargetComponent->setCustomShortName("retargetComponent");
	//retargetComponent->forceSaveValue = true;
}

SimpleConversionFilter::~SimpleConversionFilter()
{
}

void SimpleConversionFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	var prevData;
	if (autoLoadDataOnSetup) prevData = getJSONData();
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);
	if (autoLoadDataOnSetup) loadJSONData(prevData);
}

Parameter* SimpleConversionFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	if (rangeOnly)
	{
		int index = sourceParams[multiplexIndex].indexOf(source);
		jassert(index >= 0);

		Parameter* fp = filteredParameters[multiplexIndex]->getUnchecked(index);
		updateOutRange(source, fp);
		return fp;
	}

	Parameter* p = (Parameter*)ControllableFactory::getInstance()->createControllable(outTypeString);
	p->setNiceName(source->niceName);



	if (multiplexIndex != 0) return p; //only setup for 1st if multiplex multiplex

	int ghostData = !retargetComponent->getValueData().isVoid() ? retargetComponent->getValueData() : ghostOptions.getProperty("retarget", 0);
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
			retargetComponent->addOption(valueNames[i], i, false);
		}

		addExtraRetargetOptions();

		if (transferType == EXTRACT) retargetComponent->addOption("Min", MIN)->addOption("Max", MAX)->addOption("Average", AVERAGE)->addOption("Length", LENGTH)->addOption("Area", AREA);


		if (useBaseValue)
		{
			if (transferType == TARGET)
			{
				if (baseValue != nullptr && baseValue->type != p->type)
				{
					filterParams.removeControllable(baseValue);
					baseValue = nullptr;
				}

				baseValue = ControllableFactory::createParameterFrom(p, false, false);
				baseValue->setNiceName("Base Value");
				baseValue->description = "Base value for components that are not set by the conversion";
				filterParams.addParameter(baseValue);
			}
			else
			{
				if (baseValue != nullptr)
				{
					filterParams.removeControllable(baseValue);
					baseValue = nullptr;
				}
			}
		}

		retargetComponent->setValueWithData(ghostData);
	}

	updateOutRange(source, p);

	return p;
}


void SimpleConversionFilter::filterParamChanged(Parameter* p)
{
	if (p == retargetComponent && !isSettingUpSources)
	{
		for (int i = 0; i < getMultiplexCount(); i++)
		{
			for (int j = 0; j < sourceParams[i].size(); j++)
			{
				updateOutRange(sourceParams[i][j], filteredParameters[i]->getUnchecked(j));
			}
		}

		mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
	}
}

void SimpleConversionFilter::updateOutRange(Parameter* source, Parameter* out)
{
	var sourceVal = source->getValue().clone();
	if (source->hasRange())
	{
		if (out->isComplex() == source->isComplex())
		{
			if (source->hasRange() && out->canHaveRange) out->setRange(source->minimumValue, source->maximumValue);
		}
		else if (source->isComplex())
		{
			int targetData = (int)retargetComponent->getValueData();
			var newMin;
			var newMax;

			switch (targetData)
			{
			case MIN:
				newMin = INT32_MAX;
				newMax = INT32_MAX;
				for (int i = 0; i < sourceVal.size(); i++)
				{
					newMin = jmin((float)newMin, (float)source->minimumValue[i]);
					newMax = jmin((float)newMax, (float)source->maximumValue[i]);
				}
				break;

			case MAX:
				newMin = INT32_MIN;
				newMax = INT32_MIN;
				for (int i = 0; i < sourceVal.size(); i++)
				{
					newMin = jmax((float)newMin, (float)source->minimumValue[i]);
					newMax = jmax((float)newMax, (float)source->maximumValue[i]);
				}
				break;

			case AVERAGE:
				newMin = INT32_MAX;
				newMax = INT32_MIN;
				for (int i = 0; i < sourceVal.size(); i++)
				{
					newMin = jmin((float)newMin, (float)source->minimumValue[i]);
					newMax = jmax((float)newMax, (float)source->maximumValue[i]);
				}
				break;

			case LENGTH:
				newMin = 0;
				newMax = 0;
				for (int i = 0; i < sourceVal.size(); i++)
				{
					float diff = (float)source->maximumValue[i] - (float)source->minimumValue[i];
					newMax = (float)newMax + diff * diff;
				}
				newMax = sqrtf((float)newMax);
				break;

			case AREA:
				newMin = 0;
				newMax = 1;
				for (int i = 0; i < sourceVal.size(); i++)
				{
					float diff = (float)source->maximumValue[i] - (float)source->minimumValue[i];
					newMax = (float)newMax * fabsf(diff);
				}
				break;

			default:
				if (targetData < sourceVal.size())
				{
					newMin = source->minimumValue[targetData];
					newMax = source->maximumValue[targetData];
				}
				break;
			}

			out->setRange(newMin, newMax);

		}
		else
		{
			out->setRange(source->minimumValue, source->maximumValue);
		}
	}

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
		int targetData = (int)retargetComponent->getValueData();
		var val;
		switch (targetData)
		{
		case MIN:
			val = INT32_MAX;
			for (int i = 0; i < source->value.size(); i++) val = jmin((float)val, (float)source->value[i]);
			break;

		case MAX:
			val = INT32_MIN;
			for (int i = 0; i < source->value.size(); i++) val = jmax((float)val, (float)source->value[i]);
			break;

		case AVERAGE:
			val = 0;
			for (int i = 0; i < source->value.size(); i++) val = (float)val + (float)source->value[i];
			val = (float)val / source->value.size();
			break;

		case LENGTH:
			val = 0;
			for (int i = 0; i < source->value.size(); i++) val = (float)val + (float)source->value[i] * (float)source->value[i];
			val = sqrtf((float)val);
			break;

		case AREA:
			val = 1;
			for (int i = 0; i < source->value.size(); i++) val = (float)val * (float)source->value[i];
			break;

		default:
			if (targetData < source->value.size()) val = source->value[targetData];
			break;
		}

		if (val.isVoid()) return UNCHANGED;
		out->setValue(convertValue(source, val, multiplexIndex));
	}
	break;

	case TARGET:
	{
		int index = (int)retargetComponent->getValueData();

		var val = baseValue != nullptr ? baseValue->getValue().clone() : var();
		if (val.isVoid()) for (int i = 0; i < out->value.size(); ++i) val.append(0.0f);

		if (index < val.size())
		{
			val[index] = convertValue(source, source->getValue(), multiplexIndex);
		}
		out->setValue(val);
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

	var goData = ghostOptions.isObject() ? ghostOptions.clone() : var(new DynamicObject());
	goData.getDynamicObject()->setProperty("retarget", retargetComponent->getValueData());
	data.getDynamicObject()->setProperty("ghostOptions", goData);
	return data;
}

void SimpleConversionFilter::loadJSONDataItemInternal(var data)
{
	ghostOptions = data.getProperty("ghostOptions", var());
	if(ghostLinkData.isVoid()) ghostLinkData = data.getProperty("filterParams", var()).getProperty("paramLinks", var());
	MappingFilter::loadJSONDataItemInternal(data);
}

ToFloatFilter::ToFloatFilter(var params, Multiplex* multiplex) :
	SimpleConversionFilter(getTypeString(), params, FloatParameter::getTypeStringStatic(), multiplex)
{
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
	forceCase(nullptr),
	prefix(nullptr),
	suffix(nullptr),
	enumConvertMode(nullptr)
{
	format = filterParams.addEnumParameter("Format", "The format of the string");
	format->addOption("Number", NUMBER)->addOption("Time", TIME)->addOption("Hexadecimal", HEXA);
	numDecimals = filterParams.addIntParameter("Number of Decimals", "Maximum number of decimals", 3, 0, 26);
	fixedLeading = filterParams.addIntParameter("Fixed Leading", "If enabled, this will force the output to be with a certain mount of digits before the decimals", 3, 0, 100, false);
	fixedLeading->canBeDisabledByUser = true;
	forceCase = filterParams.addEnumParameter("Force Case", "If enabled, this will force the cas", false);
	forceCase->addOption("UPPERCASE", UPPER)->addOption("lowercase", LOWER)->addOption("CamelCase", UCAMEL)->addOption("lowerCamelCase", LCAMEL);
	forceCase->canBeDisabledByUser = true;

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
			if (ghostOptions.hasProperty("convertMode")) enumConvertMode->setValueWithData(ghostOptions.getDynamicObject()->getProperty("convertMode"));
		}
	}
}


var ToStringFilter::convertValue(Parameter* source, var sourceValue, int multiplexIndex)
{
	String result = "";;

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
				case HEXA:
					result += f == HEXA ? String::toHexString((int)sv) : String((float)sv, (int)filterParams.getLinkedValue(numDecimals, multiplexIndex));

					if (fixedLeading->enabled)
					{
						StringArray s;
						s.addTokens(result, ".", "");
						String first = String::repeatedString("0", (int)filterParams.getLinkedValue(fixedLeading, multiplexIndex) - s[0].length()) + s[0];
						result = first + (s.size() > 1 ? "." + s[1] : "");
					}
					break;


					break;

				case TIME:
					result += StringUtil::valueToTimeString((float)sv, numDecimals->intValue());
					break;
				}
			}
		}
	}

	result = getCasedString(result);
	result = filterParams.getLinkedValue(prefix, multiplexIndex).toString() + result + filterParams.getLinkedValue(suffix, multiplexIndex).toString();
	return result;
}

String ToStringFilter::getCasedString(const String& value)
{
	if (!forceCase->enabled) return value;
	Case c = forceCase->getValueDataAsEnum<Case>();
	switch (c)
	{
	case UPPER: return value.toUpperCase(); break;
	case LOWER: return value.toLowerCase(); break;
	case UCAMEL:
	case LCAMEL:
	{
		String lCamel = StringUtil::toShortName(value);
		if (c == LCAMEL) return lCamel;
		return lCamel.substring(0, 1).toUpperCase() + lCamel.substring(1);
	}
	break;
	}

	return value;
}

void ToStringFilter::filterParamChanged(Parameter* p)
{
	fixedLeading->hideInEditor = format->getValueDataAsEnum<Format>() != NUMBER;
	SimpleConversionFilter::filterParamChanged(p);
}

var ToStringFilter::getJSONData()
{
	var data = SimpleConversionFilter::getJSONData();
	if (enumConvertMode != nullptr) data.getProperty("ghostOptions", var()).getDynamicObject()->setProperty("convertMode", enumConvertMode->getValueData());
	return data;
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
	useBaseValue = false;
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

	if (ghostOptions.isObject() && baseColor != nullptr)
	{
		if (ghostOptions.hasProperty("color")) baseColor->setValue(ghostOptions.getDynamicObject()->getProperty("color"));
		if (ParameterLink* pLink = filterParams.getLinkedParam(baseColor))
		{
			var pLinkGhostData = ghostLinkData.getProperty("baseColor", var());
			if (!pLinkGhostData.isVoid()) pLink->loadJSONData(pLinkGhostData);

		}
		ghostOptions = var();
	}

}

void ToColorFilter::addExtraRetargetOptions()
{
	switch (transferType)
	{

	case TARGET:
		retargetComponent->addOption("Hue", HUE)->addOption("Saturation", SAT)->addOption("Brightness", VAL);
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
