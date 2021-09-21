/*
  ==============================================================================

	MergeFilter.cpp
	Created: 21 Sep 2021 9:22:16am
	Author:  bkupe

  ==============================================================================
*/

MergeFilter::MergeFilter(var params, Multiplex* multiplex)
{
	op = filterParams.addEnumParameter("Operator", "Operator to merge the input values to");
	op->addOption("Min", MIN)->addOption("Max", MAX)->addOption("Average", AVERAGE);

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
	if(!fp->isComplex())
	{

		float val = o == MIN ? (float)INT32_MAX : 0;

		for (auto& i : inputs)
		{
			if (o == AVERAGE) val += i->floatValue();
			else val = o == MIN ? jmin(val, i->floatValue()) : jmax(val, i->floatValue());
		}
		if (o == AVERAGE) val /= inputs.size();

		if (fp->checkValueIsTheSame(fp->value, val)) return UNCHANGED;
		fp->setValue(val);
	}
	else
	{
		
		var vals;
		for (int i = 0; i < fp->value.size(); i++) vals.append(o == MIN ? (float)INT32_MAX : 0);

		for (auto& i : inputs)
		{
			for (int j = 0; j < i->value.size() && j < vals.size(); j++)
			{
				float iVal = i->value[j];
				if (o == AVERAGE) vals[j] =(float)vals[j] + iVal;
				else vals[j] = o == MIN ? jmin((float)vals[j], iVal) : jmax((float)vals[j], iVal);
			}
		}
		if (o == AVERAGE)
		{
			for (int i = 0; i < vals.size(); i++) vals[i] = (float)vals[i] / inputs.size();
		}

		if (fp->checkValueIsTheSame(fp->value, vals)) return UNCHANGED;
		fp->setValue(vals);
	}

	return CHANGED;
}
