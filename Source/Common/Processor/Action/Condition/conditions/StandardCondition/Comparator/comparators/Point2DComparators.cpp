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
	addCompareOption("X >", xGreaterId);
	addCompareOption("X <", xLessId);
	addCompareOption("Y >", yGreaterId);
	addCompareOption("Y <", yLessId);;
	addCompareOption("Change", Identifier());

	updateReferenceParam();
}

Point2DComparator::~Point2DComparator()
{
}

void Point2DComparator::onContainerParameterChanged(Parameter* p)
{
	BaseComparator::onContainerParameterChanged(p);

	if (p == compareFunction)
	{
		updateReferenceParam();
	}
}

void Point2DComparator::updateReferenceParam()
{
	var od;
	Controllable::Type ot = Controllable::Type::CUSTOM;
	if (reference != nullptr)
	{
		ot = reference->type;
		od = reference->getJSONData();
		removeControllable(reference);
		reference = nullptr;
	}

	Parameter* newRef = nullptr;
	if (!currentFunctionId.isNull())
	{
		if (currentFunctionId == equalsId)
		{
			if (reference == nullptr || reference->type != Parameter::POINT2D)
			{

				newRef = new Point2DParameter("Reference", "Comparison Reference to check against source value");
				newRef->setRange(sourceParam->minimumValue, sourceParam->maximumValue);

			}
		}
		else
		{
			if (reference == nullptr || reference->type != Parameter::FLOAT)
			{
				newRef = new FloatParameter("Reference", "Comparison Reference to check against source value", 0);
			}
		}
	}

	if (newRef != nullptr)
	{
		if (newRef->type == ot) newRef->loadJSONData(od);
		else newRef->setValue(sourceParam->value, false, true, true);
		setReferenceParam(newRef);
	}
}

bool Point2DComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	if (currentFunctionId.isNull()) return true;

	Point<float> p = ((Point2DParameter*)sourceParam)->getPoint();
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->getValue();

	if (currentFunctionId == equalsId)				return p == Point<float>(value[0], value[1]);
	else if (currentFunctionId == magnGreaterId)	return p.getDistanceFromOrigin() > (float)value;
	else if (currentFunctionId == magnLessId)		return p.getDistanceFromOrigin() < (float)value;
	else if (currentFunctionId == xGreaterId)	return p.x > (float)value;
	else if (currentFunctionId == xLessId)		return p.x < (float)value;
	else if (currentFunctionId == yGreaterId)	return p.y > (float)value;
	else if (currentFunctionId == yLessId)		return p.y < (float)value;
	return false;
}