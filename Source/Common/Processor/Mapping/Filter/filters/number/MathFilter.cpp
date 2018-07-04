/*
  ==============================================================================

    MathFilter.cpp
    Created: 4 Jul 2018 2:15:50pm
    Author:  Ben

  ==============================================================================
*/

#include "MathFilter.h"

MathFilter::MathFilter(var params) :
	MappingFilter(getTypeString(),params),
	operationValue(nullptr)
{
	operation = filterParams.addEnumParameter("Operation", "The operation to use for this filter");
	operation->addOption("Offset", OFFSET)
		->addOption("Multiply", MULTIPLY)->addOption("Divide", DIVIDE)->addOption("Modulo", MODULO)
		->addOption("Floor", FLOOR)->addOption("Ceil", CEIL)->addOption("Round", ROUND);

	autoSetRange = false;
}

MathFilter::~MathFilter()
{
}

Parameter * MathFilter::setupParameterInternal(Parameter * source)
{
	Parameter * p = MappingFilter::setupParameterInternal(source);

	if (operationValue == nullptr || operationValue->type != source->type)
	{
		if (operationValue != nullptr) filterParams.removeControllable(operationValue);
		operationValue = (Parameter *)ControllableFactory::createControllable(source->getTypeString());
		operationValue->setNiceName("Value");
		filterParams.addParameter(operationValue);
	}
	
	Operation o = operation->getValueDataAsEnum<Operation>();
	operationValue->setEnabled(o != FLOOR && o != CEIL && o != ROUND);
	
	updateFilteredParamRange(p);

	return p;
}

void MathFilter::processInternal()
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	if (o != FLOOR && o != CEIL && o != ROUND)
	{
		if (operationValue == nullptr)
		{
			DBG("ERROR should not be here operationValue is null");
			return;
		}
	}

	var val = var();
	if (!sourceParam->isComplex())
	{
		val = getProcessedValue(sourceParam->value);
	} else
	{
		for (int i = 0; i < sourceParam->value.size(); i++)
		{
			val.append(getProcessedValue(sourceParam->value[i],i));
		}
	}

	filteredParameter->setValue(val);
}

void MathFilter::updateFilteredParamRange(Parameter * p)
{
	if (p == nullptr) p = filteredParameter;
	if (p == nullptr) return;

	if (sourceParam == nullptr) return;
	if (!sourceParam->hasRange() || !filteredParamShouldHaveRange())
	{
		p->clearRange();
		return;
	}

	DBG("update filtered param range");
	
	var newMin = var();
	var newMax = var();

	if (!sourceParam->isComplex())
	{
		newMin = getProcessedValue(sourceParam->minimumValue);
		newMax = jmax(getProcessedValue(sourceParam->maximumValue), (float)newMin + .001f);
	} else
	{
		for (int i = 0; i < sourceParam->value.size(); i++)
		{
			float nmin = getProcessedValue(sourceParam->minimumValue[i], i);
			float nmax = getProcessedValue(sourceParam->maximumValue[i], i);

			newMin.append(nmin);
			newMax.append(jmax(nmax, nmin + .001f));
		}
	}

	p->setRange(newMin, newMax);

}

void MathFilter::filterParamChanged(Parameter * p)
{
	if (operationValue == nullptr) return;

	if (p == operation)
	{
		Operation o = operation->getValueDataAsEnum<Operation>();
		operationValue->setEnabled(o != FLOOR && o != CEIL && o != ROUND);
	}

	if (p == operation || p == operationValue) updateFilteredParamRange();
}

float MathFilter::getProcessedValue(float val, int index)
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	
	float oVal = 0;
	if (operationValue != nullptr) oVal = index == -1 ? operationValue->floatValue() : (float)operationValue->value[index];

	if ((o == DIVIDE || o == MODULO) && oVal == 0) return 0;

	switch (o)
	{
		case OFFSET: return val + oVal;
		case MULTIPLY: return val * oVal;
		case DIVIDE: return val / oVal;
		case MODULO: return fmodf(val, oVal);

		case FLOOR: return floorf(val);
		case CEIL: return ceilf(val);
		case ROUND: return roundToInt(val);
	}

	return val;
}

bool MathFilter::filteredParamShouldHaveRange()
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	return o != FLOOR && o != CEIL && o != ROUND && o != MODULO;
}
