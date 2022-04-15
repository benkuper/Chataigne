/*
  ==============================================================================

	NumberComparators.cpp
	Created: 2 Nov 2016 8:57:34pm
	Author:  bkupe

  ==============================================================================
*/

NumberComparator::NumberComparator(Parameter* sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex),
	isFloat(dynamic_cast<FloatParameter*>(sourceParam) != nullptr)
{

	sourceRange.append(sourceParam->minimumValue);
	sourceRange.append(sourceParam->maximumValue);

	setupReferenceParam();
	reference->setValue(sourceParam->floatValue(), false, true, true);
	if (isFloat)((FloatParameter*)reference)->defaultUI = ((FloatParameter*)sourceParam)->defaultUI;

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);
	addCompareOption(">", greaterId);
	addCompareOption("<", lessId);
	addCompareOption(">=", greaterOrEqualId);
	addCompareOption("<=", lessOrEqualId);
	addCompareOption("Modulo =", moduloEqualId);
	addCompareOption("Modulo >", moduloGreaterId);
	addCompareOption("Modulo <", moduloLessId);
	addCompareOption("Floor =", floorEqualId);
	addCompareOption("Floor >", floorGreaterId);
	addCompareOption("Floor <", floorLessId);
	addCompareOption("Diff >", diffGreaterId);
	addCompareOption("Diff <", diffLessId);
	addCompareOption("Range", inRangeId);
	addCompareOption("Change", Identifier());
}

NumberComparator::~NumberComparator()
{
}

void NumberComparator::compareFunctionChanged()
{
	setupReferenceParam();
}

void NumberComparator::setupReferenceParam()
{
	if (currentFunctionId == inRangeId || currentFunctionId.toString().contains("%") || currentFunctionId.toString().contains("f"))
	{
		Point2DParameter* p2d = new Point2DParameter("Reference", "Comparison Reference to check against source value");
		p2d->setBounds(sourceRange[0], sourceRange[0], sourceRange[1], sourceRange[1]);
		if (currentFunctionId != inRangeId) p2d->setPoint(2, 0);
		setReferenceParam(p2d);
	}
	else
	{
		if (isFloat)
		{
			if (reference == nullptr || reference->type != Parameter::FLOAT)
			{
				setReferenceParam(new FloatParameter("Reference", "Comparison Reference to check against source value", 0, sourceRange[0], sourceRange[1]));
			}
		}
		else if (reference == nullptr || reference->type != Parameter::INT)
		{
			setReferenceParam(new IntParameter("Reference", "Comparison Reference to check against source value", 0, sourceRange[0], sourceRange[1]));
		}
	}
}

bool NumberComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	if (currentFunctionId.isNull()) return true;

	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->getValue();

	if (currentFunctionId == equalsId)				return sourceParam->floatValue() == (float)value;
	else if (currentFunctionId == differentId)		return sourceParam->floatValue() != (float)value;
	else if (currentFunctionId == greaterId)		return sourceParam->floatValue() > (float)value;
	else if (currentFunctionId == lessId)			return sourceParam->floatValue() < (float)value;
	else if (currentFunctionId == greaterOrEqualId)	return sourceParam->floatValue() >= (float)value;
	else if (currentFunctionId == lessOrEqualId)	return sourceParam->floatValue() <= (float)value;
	else if (currentFunctionId == moduloEqualId)	return (float)value[0] == 0 ? false : fmodf(sourceParam->floatValue(), (float)value[0]) == (float)value[1];
	else if (currentFunctionId == moduloGreaterId)	return (float)value[0] == 0 ? false : fmodf(sourceParam->floatValue(), (float)value[0]) > (float)value[1];
	else if (currentFunctionId == moduloLessId)		return (float)value[0] == 0 ? false : fmodf(sourceParam->floatValue(), (float)value[0]) < (float)value[1];
	else if (currentFunctionId == floorEqualId)		return (float)value[0] == 0 ? false : floorf(sourceParam->floatValue() / (float)value[0]) == (float)value[1];
	else if (currentFunctionId == floorGreaterId)	return (float)value[0] == 0 ? false : floorf(sourceParam->floatValue() / (float)value[0]) >(float)value[1];
	else if (currentFunctionId == floorLessId)		return (float)value[0] == 0 ? false : floorf(sourceParam->floatValue() / (float)value[0]) < (float)value[1];
	else if (currentFunctionId == inRangeId)		return sourceParam->floatValue() >= (float)value[0] && sourceParam->floatValue() <= (float)value[1];
	else if (currentFunctionId == diffGreaterId || currentFunctionId == diffLessId)
	{
		prevValues.ensureStorageAllocated(multiplexIndex + 1);
		float diff = fabsf(sourceParam->floatValue() - prevValues[multiplexIndex]);
		bool result = currentFunctionId == diffGreaterId ? diff > (float)value : diff < (float)value;
		prevValues.set(multiplexIndex, sourceParam->floatValue());
		return result;
	}
	return false;
}