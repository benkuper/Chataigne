/*
  ==============================================================================

	SimpleRemapFilter.cpp
	Created: 4 Feb 2017 5:39:07pm
	Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

SimpleRemapFilter::SimpleRemapFilter(const String& name, var params, Multiplex* multiplex) :
	MappingFilter(name, params, multiplex, true),
	targetIn(nullptr),
	targetOut(nullptr)
{
	useCustomInputRange = filterParams.addBoolParameter("Use Custom Input Range", "If enabled, the input range will be set from the source input range", false);

	targetIn = filterParams.addPoint2DParameter("Input Min/Max", "Custom input range", false);
	targetIn->setPoint(0, 1);

	targetOut = filterParams.addPoint2DParameter("Target Min/Max", "New maximum for output");
	targetOut->isCustomizableByUser = false;
	targetOut->setPoint(0, 1);

	forceFloatOutput = filterParams.addBoolParameter("Force Float", "If checked, this will force transforming integer in floats", true);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::BOOL, Controllable::POINT2D, Controllable::POINT3D);
	autoSetRange = false;
}

SimpleRemapFilter::SimpleRemapFilter(var params, Multiplex* multiplex) :
	SimpleRemapFilter("Remap", params, multiplex) //using getTypeString makes mac crash...
{

}

SimpleRemapFilter::~SimpleRemapFilter()
{
}

bool SimpleRemapFilter::setupSources(Array<Parameter*> sources, int multiplexIndex, bool rangeOnly)
{
	bool result = MappingFilter::setupSources(sources, multiplexIndex, rangeOnly);
	computeOutRanges();
	return result;
}

Parameter* SimpleRemapFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;

	if(rangeOnly) return nullptr; //nothing to do, range is forced from remap
	
	if (!source->isComplex() && forceFloatOutput->boolValue())
	{
		p = new FloatParameter(source->niceName, source->description, source->getValue().clone(), source->minimumValue, source->maximumValue);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	}

	if (!useCustomInputRange->isOverriden || !useCustomInputRange->boolValue()) useCustomInputRange->setValue(!source->hasRange());
	if (p->isComplex())
	{
		var minVal;
		var maxVal;
		int numVals = p->getValue().size();
		for (int i = 0; i < numVals; i++)
		{
			minVal.append(jmin<float>(targetOut->x, targetOut->y));
			maxVal.append(jmax<float>(targetOut->x, targetOut->y));
		}
		p->setRange(minVal, maxVal);
	}
	else
	{
		p->setRange(jmin<float>(targetOut->x, targetOut->y), jmax<float>(targetOut->x, targetOut->y));
	}
	return p;
}

MappingFilter::ProcessResult  SimpleRemapFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (targetIn == nullptr || targetOut == nullptr || out == nullptr) return STOP_HERE;

	var remappedValue = getRemappedValueFor(source, multiplexIndex);
	out->setValue(remappedValue);

	return CHANGED;
}

var SimpleRemapFilter::getRemappedValueFor(Parameter* source, int multiplexIndex)
{
	var sourceVal = source->getValue().clone(); //copy the value to avoid access problems
	var targetVal = sourceVal;

	var linkOut = filterParams.getLinkedValue(targetOut, multiplexIndex);

	if (linkOut[0] == linkOut[1])
	{
		if (sourceVal.isArray())
		{
			var tVal;
			for (int i = 0; i < sourceVal.size(); i++) tVal.append(linkOut[0]);
			return tVal;
		}
		else
		{
			return linkOut[0];
		}
	}

	var tOut;
	if (sourceVal.isArray())
	{
		for (int i = 0; i < sourceVal.size(); i++)
		{
			var ttOut;
			ttOut.append(linkOut[0]);
			ttOut.append(linkOut[1]);
			tOut.append(ttOut);
		}
	}
	else
	{
		tOut = linkOut;

	}


	var linkIn = filterParams.getLinkedValue(targetIn, multiplexIndex);

	var tIn;
	if (!source->hasRange() || useCustomInputRange->boolValue())
	{
		if (sourceVal.isArray())
		{
			for (int i = 0; i < sourceVal.size(); i++)
			{
				var ttIn;
				ttIn.append(linkIn[0]);
				ttIn.append(linkIn[1]);
				tIn.append(ttIn);
			}
			DBG(tIn.size() << " / " << sourceVal.size());
		}
		else
		{
			tIn = linkIn;
		}
	}
	else
	{
		if (sourceVal.isArray())
		{
			for (int i = 0; i < sourceVal.size(); i++)
			{
				var ttIn;
				ttIn.append(source->minimumValue[i]);
				ttIn.append(source->maximumValue[i]);
				tIn.append(ttIn);
			}
		}
		else
		{
			tIn.append(source->minimumValue);
			tIn.append(source->maximumValue);
		}
	}


	if (sourceVal.isArray())
	{
		var targetVal;
		for (int i = 0; i < sourceVal.size(); i++)
		{
			var ttIn = tIn[i];
			var ttOut = tOut[i];

			if (ttIn[0] != ttIn[1] && ttOut[0] != ttOut[1])
			{
				float targetValue = jmap((float)sourceVal[i], (float)ttIn[0], (float)ttIn[1], (float)ttOut[0], (float)ttOut[1]);
				targetVal.append(targetValue);
			}
			else
			{
				targetVal.append(sourceVal[i]);
			}
		}

		return targetVal;
	}
	else
	{
		if (tIn[0] != tIn[1] && tOut[0] != tOut[1])
		{
			return jmap(source->floatValue(), (float)tIn[0], (float)tIn[1], (float)tOut[0], (float)tOut[1]);
		}
	}

	return sourceVal;
}

void SimpleRemapFilter::computeOutRanges()
{
	bool hasChanged = false;
	for (int i = 0; i < filteredParameters.size(); i++)
	{
		auto mFilteredParams = filteredParameters[i];

		for (int j = 0; j < mFilteredParams->size(); j++)
		{
			Parameter* f = mFilteredParams->getUnchecked(j);
			if (!isChannelEligible(j))
			{
				if (Parameter* source = sourceParams[i][j]) f->setRange(source->minimumValue, source->maximumValue);
				continue;
			}
			
			var mRange = filterParams.getLinkedValue(targetOut, i);

			if (f->type == Controllable::FLOAT || f->type == Controllable::INT)
			{
				var newMin = jmin<float>(mRange[0], mRange[1]);
				var newMax = jmax<float>(mRange[0], mRange[1]);
				if (newMin != f->minimumValue || newMax != f->maximumValue) hasChanged = true;
				f->setRange(newMin, newMax);
			}
			else if (f->type == Controllable::POINT2D || f->type == Controllable::POINT3D)
			{
				var minVal;
				var maxVal;
				for (int i = 0; i < f->value.size(); i++)
				{
					minVal.append(jmin<float>(mRange[0], mRange[1]));
					maxVal.append(jmax<float>(mRange[0], mRange[1]));
				}

				if (f->checkValueIsTheSame(minVal, f->minimumValue) || f->checkValueIsTheSame(maxVal, f->maximumValue)) hasChanged = true;
				f->setRange(minVal, maxVal);

			}
		}
	}

	if(hasChanged) mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
}

void SimpleRemapFilter::filterParamChanged(Parameter* p)
{
	if (p == useCustomInputRange)
	{
		targetIn->setEnabled(useCustomInputRange->boolValue());
	}
	else if (p == targetOut)
	{
		computeOutRanges();
	}
	else if (p == forceFloatOutput)
	{
		if (!isCurrentlyLoadingData) setupParametersInternal(-1);

		mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
		filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}
}
