/*
  ==============================================================================

    CropFilter.cpp
    Created: 26 Jan 2018 11:39:14am
    Author:  Ben

  ==============================================================================
*/


CropFilter::CropFilter(var params, Multiplex * multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{

	targetMin = filterParams.addFloatParameter("Target Min", "New minimum for output", 0, 0, 1);
	targetMax = filterParams.addFloatParameter("Target Max", "New maximum for output", 1, 0, 1);
	targetMin->defaultUI = FloatParameter::LABEL;
	targetMax->defaultUI = FloatParameter::LABEL;
	targetMin->isCustomizableByUser = false;
	targetMax->isCustomizableByUser = false;

	autoSetRange = false;

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT);
}

CropFilter::~CropFilter()
{
}

Parameter * CropFilter::setupSingleParameterInternal(Parameter * source, int multiplexIndex, bool rangeOnly)
{
	float tMin = filterParams.getLinkedValue(targetMin, multiplexIndex);
	float tMax = filterParams.getLinkedValue(targetMax, multiplexIndex);

	Parameter * p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	if (p->isComplex())
	{
		var minVal;
		var maxVal;
		for (int i = 0; i < p->value.size(); ++i)
		{
			minVal.append(tMin);
			maxVal.append(jmax<float>(tMax, tMin));
		}

		p->setRange(minVal, maxVal);
	}
	else
	{
		p->setRange(tMin, jmax<float>(tMax, tMin));
	}
	return p;
}


MappingFilter::ProcessResult CropFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	out->setValue(source->value);
	return CHANGED;
}

void CropFilter::filterParamChanged(Parameter* p)
{
	if (p == targetMin || p == targetMax)
	{
		for (auto& mFilteredParams : filteredParameters)
		{
			for (auto& fp : *mFilteredParams)
			{
				if (fp == nullptr) continue;
				if (!filterTypeFilters.contains(fp->type)) continue;
				fp->setRange(targetMin->floatValue(), jmax<float>(targetMax->floatValue(), targetMin->floatValue()));
			}
			
		}
	}
}