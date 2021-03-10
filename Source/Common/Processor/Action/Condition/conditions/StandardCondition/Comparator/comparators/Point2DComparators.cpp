/*
  ==============================================================================

	Point2DComparators.cpp
	Created: 2 Nov 2016 8:58:14pm
	Author:  bkupe

  ==============================================================================
*/

Point2DComparator::Point2DComparator(Parameter* sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex),
	sourceParam(sourceParam)
{
	addCompareOption("=", equalsId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);

	updateReferenceParam();
}

Point2DComparator::~Point2DComparator()
{
}

void Point2DComparator::onContainerParameterChanged(Parameter* p)
{
	if (p == compareFunction)
	{
		updateReferenceParam();
	}

	BaseComparator::onContainerParameterChanged(p);
}

void Point2DComparator::updateReferenceParam()
{
	if (reference != nullptr)
	{
		removeControllable(reference);
		reference = nullptr;
	}

	if (currentFunctionId == equalsId)
	{
		if (reference == nullptr || reference->type != Parameter::POINT2D)
		{

			setReferenceParam(new Point2DParameter("Reference", "Comparison Reference to check against source value"));
			reference->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
			reference->setValue(sourceParam->value, false, true, true);
		}
	}
	else
	{
		if (reference == nullptr || reference->type != Parameter::FLOAT)
		{
			setReferenceParam(new FloatParameter("Reference", "Comparison Reference to check against source value", 0, 0));
		}
	}

}

bool Point2DComparator::compare(Parameter* sourceParam, int multiplexIndex)
{
	Point<float> p = ((Point2DParameter*)sourceParam)->getPoint();
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->getValue();

	if (currentFunctionId == equalsId)				return p == Point<float>(value[0], value[1]);
	else if (currentFunctionId == magnGreaterId)	return p.getDistanceFromOrigin() > (float)value;
	else if (currentFunctionId == magnLessId)		return p.getDistanceFromOrigin() < (float)value;
	return false;
}