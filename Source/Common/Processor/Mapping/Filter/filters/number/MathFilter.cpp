/*
  ==============================================================================

    MathFilter.cpp
    Created: 4 Jul 2018 2:15:50pm
    Author:  Ben

  ==============================================================================
*/

MathFilter::MathFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true),
	operationValue(nullptr)
{
	operation = filterParams.addEnumParameter("Operation", "The operation to use for this filter");
	operation->addOption("Offset", OFFSET)
		->addOption("Multiply", MULTIPLY)->addOption("Divide", DIVIDE)->addOption("Modulo", MODULO)
		->addOption("Floor", FLOOR)->addOption("Ceil", CEIL)->addOption("Round", ROUND)->addOption("Max",MAX)->addOption("Min",MIN)
		->addOption("Absolute", ABSOLUTE);

	autoSetRange = false;
	rangeRemapMode = filterParams.addEnumParameter("Range Remap Mode", "How to setup the output range.\nKeep will keep the input's range, adjust will ajdust automatically depending on the operator. \
													\nFree will remove the range");
	rangeRemapMode->addOption("Free", FREE)->addOption("Adjust", AJDUST)->addOption("Keep", KEEP);

	filterTypeFilters.add(Controllable::FLOAT, Controllable::INT, Controllable::POINT2D, Controllable::POINT3D);
}

MathFilter::~MathFilter()
{
}

void MathFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);

	if (multiplexIndex != 0) return; //only setup depending on first value

	if (!rangeOnly)
	{
		Array<WeakReference<Parameter>> mSourceParams = sourceParams[multiplexIndex];
		if (mSourceParams.size() == 0 || mSourceParams[0] == nullptr) return;

		if (operationValue == nullptr || operationValue->type != mSourceParams[0]->type)
		{
			if (operationValue != nullptr)
			{
				opValueData = operationValue->getJSONData();
				filterParams.removeControllable(operationValue);
			}
			bool loadLastData = (operationValue == nullptr && opValueData.isObject()) || (operationValue != nullptr && mSourceParams[0]->type == operationValue->type);
			operationValue = (Parameter*)ControllableFactory::createControllable(mSourceParams[0]->getTypeString());
			operationValue->setNiceName("Value");
			if (loadLastData) operationValue->loadJSONData(opValueData);
			//operationValue->isSavable = false;
			filterParams.addParameter(operationValue);
		}

		Operation o = operation->getValueDataAsEnum<Operation>();
		operationValue->setEnabled(o != FLOOR && o != CEIL && o != ROUND && o != ABSOLUTE);
	}
	
	updateFilteredParamsRange();
}

MappingFilter::ProcessResult  MathFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	//Operation o = operation->getValueDataAsEnum<Operation>();

	var val = var();
	if (!source->isComplex())
	{
		val = getProcessedValue(source->value, -1, multiplexIndex);
	} else
	{
		for (int i = 0; i < source->value.size(); ++i)
		{
			val.append(getProcessedValue(source->value[i], i, multiplexIndex));
		}
	}

	out->setValue(val);
	return CHANGED;
}

bool MathFilter::updateFilteredParamsRange()
{
	bool hasChanged = false;

	for (int i = 0; i < filteredParameters[0]->size(); ++i)
	{
		Parameter* sourceParam = sourceParams[0][i];
		if (sourceParam == nullptr) continue;

		if (!filterTypeFilters.contains(sourceParam->type)) continue;

		Parameter* p = filteredParameters[0]->getUnchecked(i);
		if (p == nullptr) continue;

		RangeRemapMode rm = rangeRemapMode->getValueDataAsEnum<RangeRemapMode>();

		if (rm == FREE || !sourceParam->hasRange() || !filteredParamShouldHaveRange())
		{
			p->clearRange();
			hasChanged = true; 
			continue;
		}

		if (rm == KEEP)
		{
			p->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
			hasChanged = true;
			continue;
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

		case ABSOLUTE:
			newMin = 0;
			newMax = jmax(0.f, (float)sourceParam->maximumValue);
			break;

		default:
		{
			if (!sourceParam->isComplex())
			{

				float nmin = getProcessedValue(sourceParam->minimumValue, -1, 0);
				float nmax = getProcessedValue(sourceParam->maximumValue, -1, 0);
				newMin = jmin(nmin, nmax);
				newMax = jmax(nmin, nmax);
			}
			else
			{
				for (int j = 0; j < sourceParam->value.size(); j++)
				{
					float nmin = getProcessedValue(sourceParam->minimumValue[j], j, 0);
					float nmax = getProcessedValue(sourceParam->maximumValue[j], j, 0);

					newMin.append(jmin(nmin, nmax));
					newMax.append(jmax(nmin, nmax));
				}
			}
		}
		break;
		}

		p->setRange(newMin, newMax);
		hasChanged = true;
	}

	return hasChanged;
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
		bool hasChanged = updateFilteredParamsRange();
		if (hasChanged)
		{
			mappingFilterListeners.call(&FilterListener::filteredParamRangeChanged, this);
			if (p == rangeRemapMode) filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
		}
	}
}

void MathFilter::parameterControlModeChanged(Parameter* p)
{
	if (p == operationValue)
	{
		if (operationValue->controlMode != Parameter::MANUAL) rangeRemapMode->setValue(FREE); //force free to avoid constant out recreation. Better way would be to have rangeUpdate accross filter and output to avoid recreating the whole parameters
		return;
	}

	MappingFilter::parameterControlModeChanged(p);

}

float MathFilter::getProcessedValue(float val, int index, int multiplexIndex)
{
	Operation o = operation->getValueDataAsEnum<Operation>();
	
	float oVal = 0;
	if (operationValue != nullptr)
	{
		if (!operationValue->isComplex())
		{
			oVal = filterParams.getLinkedValue(operationValue, multiplexIndex);
		}
		else
		{
			oVal = (float)operationValue->value[(index + operationValue->value.size()) % operationValue->value.size()];
		}
	}

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
		case ABSOLUTE: return std::fabs(val);
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
	opValueData = data.getProperty("operationValue", var());
	MappingFilter::loadJSONDataInternal(data);
}
