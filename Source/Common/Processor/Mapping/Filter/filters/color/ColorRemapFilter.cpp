/*
  ==============================================================================

	ColorRemapFilter.cpp
	Created: 29 Mar 2021 6:22:21pm
	Author:  bkupe

  ==============================================================================
*/

ColorRemapFilter::ColorRemapFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true),
	targetIn(nullptr)
{
	useCustomInputRange = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);

	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range", false);
	targetIn->setPoint(0, 1);
	addChildControllableContainer(&gradientManager);
	gradientManager.setLength(1);
	gradientManager.allowKeysOutside = false;

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::BOOL, Controllable::COLOR, Controllable::ENUM);
}

ColorRemapFilter::~ColorRemapFilter()
{
}

Parameter* ColorRemapFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	if (rangeOnly) return nullptr; //nothing to do, range is forced from remap
	return new ColorParameter("Out", "Out color from remap", Colours::black);
}

MappingFilter::ProcessResult  ColorRemapFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (targetIn == nullptr || out == nullptr) return STOP_HERE;

	var remappedValue = getRemappedValueFor(source, multiplexIndex);
	out->setValue(remappedValue);

	return CHANGED;
}

var ColorRemapFilter::getRemappedValueFor(Parameter* source, int multiplexIndex)
{
	var sourceVal = source->getValue();
	if (source->type == Parameter::COLOR) sourceVal = ((ColorParameter*)source)->getColor().getBrightness();
	else if (source->type == Parameter::ENUM)
	{
		int index = ((EnumParameter*)source)->getIndexForKey(((EnumParameter*)source)->getValueKey());
		sourceVal = index;
	}
	var linkIn = filterParams.getLinkedValue(targetIn, multiplexIndex);

	var tIn;
	if (!source->hasRange() || useCustomInputRange->boolValue())
	{
		tIn = linkIn;
	}
	else
	{
		if (source->isComplex())
		{
			tIn.append(source->minimumValue[0]);
			tIn.append(source->maximumValue[0]);
		}
		else
		{
			tIn.append(source->minimumValue);
			tIn.append(source->maximumValue);
		}
	}


	float targetValue = jmap((float)sourceVal, (float)tIn[0], (float)tIn[1], 0.0f, 1.0f);
	Colour c = gradientManager.getColorForPosition(targetValue);
	var cVal;
	cVal.append(c.getFloatRed());
	cVal.append(c.getFloatGreen());
	cVal.append(c.getFloatBlue());
	cVal.append(c.getFloatAlpha());

	return cVal;
}

void ColorRemapFilter::filterParamChanged(Parameter* p)
{
	MappingFilter::filterParamChanged(p);

	if (p == useCustomInputRange)
	{
		targetIn->setEnabled(useCustomInputRange->boolValue());
	}
}

var ColorRemapFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty("gradient", gradientManager.getJSONData());
	return data;
}

void ColorRemapFilter::loadJSONDataItemInternal(var data)
{
	MappingFilter::loadJSONDataItemInternal(data);
	gradientManager.loadJSONData(data.getProperty("gradient", var()));
}
