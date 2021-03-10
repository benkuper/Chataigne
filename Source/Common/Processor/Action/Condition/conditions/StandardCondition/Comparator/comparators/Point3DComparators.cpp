/*
  ==============================================================================

	Point3DComparators.cpp
	Created: 2 Nov 2016 8:58:22pm
	Author:  bkupe

  ==============================================================================
*/

Point3DComparator::Point3DComparator(Parameter* sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex),
	sourceParam(sourceParam)
{


	addCompareOption("=", equalsId);
	addCompareOption("Magnitude >", magnGreaterId);
	addCompareOption("Magnitude <", magnLessId);

	updateReferenceParam();
}

Point3DComparator::~Point3DComparator()
{
}

void Point3DComparator::onContainerParameterChanged(Parameter* p)
{
	if (p == compareFunction)
	{
		updateReferenceParam();
	}

	BaseComparator::onContainerParameterChanged(p);
}

void Point3DComparator::updateReferenceParam()
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
			setReferenceParam(new Point3DParameter("Reference", "Comparison Reference to check against source value", sourceParam->value));
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

bool Point3DComparator::compare(Parameter* sourceParam, int multiplexIndex)
{

	Vector3D<float> p = ((Point3DParameter*)sourceParam)->getVector();
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->getValue();

	if (currentFunctionId == equalsId)				return p.x == (float)value[0] && p.y == (float)value[1] && p.z == (float)value[2];
	else if (currentFunctionId == magnGreaterId)	return p.length() > (float)value;
	else if (currentFunctionId == magnLessId)		return p.length() < (float)value;
	
	return false;
}
