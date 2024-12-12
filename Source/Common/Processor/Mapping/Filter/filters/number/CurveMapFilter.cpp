/*
  ==============================================================================

    CurveMapFilter.cpp
    Created: 4 Feb 2017 5:39:24pm
    Author:  Ben

  ==============================================================================
*/

CurveMapFilter::CurveMapFilter(var params, Multiplex* multiplex) :
	SimpleRemapFilter(getTypeString(), params, multiplex),
	curve("Curve")
{
	curve.isSelectable = false;
	curve.length->setValue(1);
	curve.addKey(0, 0, false);
	curve.items[0]->easingType->setValueWithData(Easing::BEZIER);
	curve.addKey(1, 1, false);
	//curve.enableSnap->setValue(false);
	curve.selectItemWhenCreated = false;
	curve.hideEditorHeader = true;
	filterParams.addChildControllableContainer(&curve);


	filterTypeFilters.add(Controllable::INT, Controllable::FLOAT, Controllable::POINT2D, Controllable::POINT3D);
}

CurveMapFilter::~CurveMapFilter()
{
}

MappingFilter::ProcessResult CurveMapFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (targetIn == nullptr || targetOut == nullptr || out == nullptr) return STOP_HERE;

	var remappedVal = getRemappedValueFor(source, multiplexIndex);

	if (multiplexIndex == getPreviewIndex() && source == sourceParams[0].getFirst())
	{
		float normVal = 0;
		if (out->isComplex()) normVal = jmap<float>(remappedVal[0], (float)out->minimumValue[0], (float)out->maximumValue[0], 0.f, 1.f);
		else normVal = jmap<float>((float)remappedVal, out->minimumValue, out->maximumValue, 0.f, 1.f);

		curve.position->setValue(normVal); //for feedback
	}

	if (source->isComplex())
	{
		var normCurveVal;
		var sourceVal = source->getValue().clone();
		for (int i = 0; i < sourceVal.size(); i++)
		{
			float normVal = jmap<float>(remappedVal[i], (float)out->minimumValue[i], (float)out->maximumValue[i], 0.f, 1.f);
			normCurveVal.append(curve.getValueAtPosition(normVal));
		}
		out->setNormalizedValue(normCurveVal);
	}
	else
	{
		float normVal = jmap<float>((float)remappedVal, out->minimumValue, out->maximumValue, 0.f, 1.f); 
		out->setNormalizedValue(curve.getValueAtPosition(normVal));
	}

	return CHANGED;
}

void CurveMapFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (c == curve.value || c == curve.position) return; //avoid value change to be notifying the mapping, it would be recognized as a filter parameter and would trigger a new process
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);
}

var CurveMapFilter::getJSONData(bool includeNonOverriden)
{
	var data = MappingFilter::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty(curve.shortName, curve.getJSONData());
	return data;
}

void CurveMapFilter::loadJSONDataInternal(var data)
{
	MappingFilter::loadJSONDataInternal(data);
	curve.loadJSONData(data.getProperty(curve.shortName, var()), true);
}