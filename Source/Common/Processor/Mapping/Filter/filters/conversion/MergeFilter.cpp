/*
  ==============================================================================

	MergeFilter.cpp
	Created: 21 Sep 2021 9:22:16am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MergeFilter::MergeFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	autoSetRange = false;
	op = filterParams.addEnumParameter("Operator", "Operator to merge the input values to");
	op->addOption("Min", MIN)->addOption("Max", MAX)->addOption("Average", AVERAGE)->addOption("Sum", SUM)->addOption("Multiply", MULTIPLY)->addOption("Distance", DISTANCE)->addOption("Difference", DIFF);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::BOOL, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

MergeFilter::~MergeFilter()
{
}

void MergeFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	if (rangeOnly) return;
	Array<WeakReference<Parameter>> sParams = sourceParams[multiplexIndex];

	if (sParams.size() == 0)
	{
		filteredParameters[multiplexIndex]->clear();
		return;
	}

	Controllable::Type sType = sParams[0]->type;
	if (filteredParameters[multiplexIndex]->size() > 0)
	{
		Parameter* fParam = filteredParameters[multiplexIndex]->getUnchecked(0);
		if (fParam != nullptr && fParam->type == sType) return;
	}


	filteredParameters[multiplexIndex]->clear();
	Parameter* p = (Parameter*)ControllableFactory::createControllable(sParams[0]->getTypeString());
	p->setNiceName("Out " + String(multiplexIndex + 1));
	filteredParameters[multiplexIndex]->add(p);
}

MappingFilter::ProcessResult MergeFilter::processInternal(Array<Parameter*> inputs, int multiplexIndex)
{
	if (inputs.size() == 0 || filteredParameters[multiplexIndex]->size() == 0) return ProcessResult::STOP_HERE;

	Parameter* fp = filteredParameters[multiplexIndex]->getUnchecked(0);

	Operator o = op->getValueDataAsEnum<Operator>();
	if (!fp->isComplex())
	{

		float val = 0;

		if (o == MIN) val = (float)INT32_MAX;
		else if (o == MULTIPLY) val = 1;
		else if (o == MAX) val = (float)INT32_MIN;
		else if (o == DIFF || o == DISTANCE) val = inputs[0]->floatValue();

		float minVal = (float)INT32_MAX;
		float maxVal = (float)INT32_MIN;

		for (int i = 0; i < inputs.size(); i++)
		{
			if (i == 0 && (o == DIFF || o == DISTANCE)) continue;

			switch (o)
			{
			case AVERAGE:
			case SUM: val += inputs[i]->floatValue(); break;
			case MULTIPLY: val *= inputs[i]->floatValue(); break;
			case MIN: val = jmin(val, inputs[i]->floatValue()); break;
			case MAX: val = jmax(val, inputs[i]->floatValue()); break;
			case DIFF: val -= inputs[i]->floatValue(); break;
			case DISTANCE:
				minVal = jmin(minVal, inputs[i]->floatValue());
				maxVal = jmax(maxVal, inputs[i]->floatValue());
				break;
			}
		}

		if (o == AVERAGE) val /= inputs.size();
		else if (o == DISTANCE) val = maxVal - minVal;

		if (fp->checkValueIsTheSame(fp->value, val)) return UNCHANGED;
		fp->setValue(val);
	}
	else
	{

		var vals;
		var minVals;
		var maxVals;
		for (int i = 0; i < fp->value.size(); i++)
		{
			minVals.append((float)INT32_MIN);
			maxVals.append((float)INT32_MAX);

			if (o == SUM || o == AVERAGE) vals.append(0);
			else if (o == MIN) vals.append((float)INT32_MAX);
			else if (o == MULTIPLY) vals.append(1);
			else if (o == MAX) vals.append((float)INT32_MIN);
			else if (o == DIFF || o == DISTANCE) vals.append(inputs[0]->value[i]);
		}

		for (int i = 0; i < inputs.size(); i++)
		{
			if (i == 0 && (o == DIFF || o == DISTANCE)) continue;

			Parameter* in = inputs[i];
			for (int j = 0; j < in->value.size() && j < vals.size(); j++)
			{
				//if (i == 0 && (o == DIFF || o == DISTANCE)) continue;

				float vj = vals[j].clone();
				float ij = in->value[j];

				switch (o)
				{
				case AVERAGE:
				case SUM: vals[j] = vj + ij; break;
				case MULTIPLY: vals[j] = vj * ij; break;
				case MIN: vals[j] = jmin<float>(vj, ij); break;
				case MAX: vals[j] = jmax<float>(vj, ij); break;
				case DIFF: vals[j] = vj - ij; break;
				case DISTANCE:
					minVals[j] = jmin<float>(minVals[j], ij);
					maxVals[j] = jmax<float>(maxVals[j], ij);
					break;
				}
			}
		}

		if (o == AVERAGE || o == DISTANCE)
		{
			for (int i = 0; i < vals.size(); i++)
			{
				if (o == AVERAGE) vals[i] = (float)vals[i] / inputs.size();
				else if (o == DISTANCE) vals[i] = (float)maxVals[i] - (float)minVals[i];
			}
		}

		if (fp->checkValueIsTheSame(fp->value, vals)) return UNCHANGED;
		fp->setValue(vals);
	}

	return CHANGED;
}
