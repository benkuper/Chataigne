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
		->addOption("Floor", FLOOR)->addOption("Ceil", CEIL)->addOption("Round", ROUND)->addOption("Max",MAX)->addOption("Min",MIN);

	autoSetRange = false;
	rangeRemapMode = filterParams.addEnumParameter("Range Remap Mode", "How to setup the output range.\nKeep will keep the input's range, adjust will ajdust automatically depending on the operator. \
													\nFree will remove the range");
	rangeRemapMode->addOption("Adjust", AJDUST)->addOption("Keep", KEEP)->addOption("Free", FREE);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::POINT2D, Controllable::POINT3D);
}

MathFilter::~MathFilter()
{
}

void MathFilter::setupParametersInternal()
{
	MappingFilter::setupParametersInternal();

	if (sourceParams.size() == 0 || sourceParams[0] == nullptr) return;

	if (operationValue == nullptr || operationValue->type != sourceParams[0]->type)
	{
		if (operationValue != nullptr)
		{
			opValueData = operationValue->getJSONData();
			filterParams.removeControllable(operationValue);
		}
		bool sameType = operationValue != nullptr && sourceParams[0]->type == operationValue->type;
		operationValue = (Parameter*)ControllableFactory::createControllable(sourceParams[0]->getTypeString());
		operationValue->setNiceName("Value");
		if(sameType) operationValue->loadJSONData(opValueData);
		operationValue->isSavable = false;
		filterParams.addParameter(operationValue);
	}

	Operation o = operation->getValueDataAsEnum<Operation>();
	operationValue->setEnabled(o != FLOOR && o != CEIL && o != ROUND);


	updateFilteredParamsRange();
}

void MathFilter::processSingleParameterInternal(Parameter * source, Parameter * out)
{
	//Operation o = operation->getValueDataAsEnum<Operation>();

	var val = var();
	if (!source->isComplex())
	{
		val = getProcessedValue(source->value);
	} else
	{
		for (int i = 0; i < source->value.size(); ++i)
		{
			val.append(getProcessedValue(source->value[i],i));
		}
	}

	out->setValue(val);
}

void MathFilter::updateFilteredParamsRange()
{
	for (int i=0;i<filteredParameters.size(); ++i)
	{
		Parameter* sourceParam = sourceParams[i];
		if (sourceParam == nullptr) continue;

		if (!filterTypeFilters.contains(sourceParam->type)) continue;

		Parameter* p = filteredParameters[i];
		if (p == nullptr) continue;

		RangeRemapMode rm = rangeRemapMode->getValueDataAsEnum<RangeRemapMode>();

		if (rm == FREE || !sourceParam->hasRange() || !filteredParamShouldHaveRange())
		{
			p->clearRange();
			continue;
		}

		if (rm == KEEP)
		{
			p->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
			return;
		}


		//Only in RangeRemapMode::ADJUST

		var newMin = var();
		var newMax = var();
		
		Operation o = operation->getValueDataAsEnum<Operation>();
		switch (o)
		{

		case MODULO:
			newMin = jmin(0.f, operationValue->floatValue());
			newMax = jmax(0.f, operationValue->floatValue());
			break;

		default:
		{
			if (!sourceParam->isComplex())
			{

				float nmin = getProcessedValue(sourceParam->minimumValue);
				float nmax = getProcessedValue(sourceParam->maximumValue);
				newMin = jmin(nmin, nmax);
				newMax = jmax(nmin, nmax);
			}
			else
			{
				for (int j = 0; j < sourceParam->value.size(); j++)
				{
					float nmin = getProcessedValue(sourceParam->minimumValue[j], j);
					float nmax = getProcessedValue(sourceParam->maximumValue[j], j);

					newMin.append(jmin(nmin, nmax));
					newMax.append(jmax(nmin, nmax));
				}
			}
		}
		break;
		}

		p->setRange(newMin, newMax);
	}
}

void MathFilter::filterParamChanged(Parameter * p)
{
	if (operationValue == nullptr) return;

	if (p == operation)
	{
		Operation o = operation->getValueDataAsEnum<Operation>();
		operationValue->setEnabled(o != FLOOR && o != CEIL && o != ROUND);
	}

	RangeRemapMode rm = rangeRemapMode->getValueDataAsEnum<RangeRemapMode>();
	if (p == operation || (p == operationValue && rm == RangeRemapMode::AJDUST)|| p == rangeRemapMode)
	{
		updateFilteredParamsRange();
		mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
		if(p == rangeRemapMode) filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}
}

float MathFilter::getProcessedValue(float val, int index)
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	
	float oVal = 0;
	if (operationValue != nullptr && operationValue->value.size() > 0) oVal = (index == -1 && !operationValue->isComplex())? operationValue->floatValue() : (float)operationValue->value[index%operationValue->value.size()];

	if ((o == DIVIDE || o == MODULO) && oVal == 0) return 0;

	switch (o)
	{
		case OFFSET: return val + oVal;
		case MULTIPLY: return val * oVal;
		case DIVIDE: return oVal > 0 ? val / oVal : (val / -oVal)*-1;
		case MODULO: return fmodf(val, abs(oVal));

		case FLOOR: return floorf(val);
		case CEIL: return ceilf(val);
		case ROUND: return roundToInt(val);
		case MAX: return std::max(oVal, val);
		case MIN: return std::min(oVal, val);
	}

	return val;
}

bool MathFilter::filteredParamShouldHaveRange()
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	return o != FLOOR && o != CEIL && o != ROUND;
}

var MathFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	if (operationValue != nullptr) data.getDynamicObject()->setProperty("operationValue", operationValue->getJSONData());
	return data;
}

void MathFilter::loadJSONDataInternal(var data)
{
	MappingFilter::loadJSONDataInternal(data);
	opValueData = data.getProperty("operationValue", var());
	
}
