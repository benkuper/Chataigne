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
	addCompareOption("X >", xGreaterId);
	addCompareOption("X <", xLessId);
	addCompareOption("Y >", yGreaterId);
	addCompareOption("Y <", yLessId);
	addCompareOption("Z >", zGreaterId);
	addCompareOption("Z <", zLessId);
	addCompareOption("Change", changeId);
	updateReferenceParam();
}

Point3DComparator::~Point3DComparator()
{
}

void Point3DComparator::onContainerParameterChanged(Parameter* p)
{
	BaseComparator::onContainerParameterChanged(p);

	if (p == compareFunction)
	{
		updateReferenceParam();
	}
}

void Point3DComparator::updateReferenceParam()
{
	var od;
	Controllable::Type ot = Controllable::Type::CUSTOM;
	if (reference != nullptr)
	{
		ot = reference->type;
		od = reference->getJSONData();
		//removeControllable(reference);
		//GenericScopedLock lock(compareLock);
		//reference = nullptr;
	}

	Parameter* newRef = nullptr;

	if (currentFunctionId != changeId)
	{
		if (currentFunctionId == equalsId)
		{
			if (reference == nullptr || reference->type != Parameter::POINT2D)
			{
				newRef = new Point3DParameter("Reference", "Comparison Reference to check against source value");
				newRef->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
			}
			else
			{
				newRef = reference;
			}
		}
		else
		{
			if (reference == nullptr || reference->type != Parameter::FLOAT)
			{
				newRef = new FloatParameter("Reference", "Comparison Reference to check against source value", 0);
			}
			else
			{
				newRef = reference;
			}
		}
	}

	if (newRef == reference) return;

	if (newRef != nullptr)
	{
		if (newRef->type == ot) newRef->loadJSONData(od);
		else newRef->setValue(sourceParam->value, false, true, true);
	}

	setReferenceParam(newRef);
}

bool Point3DComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	Vector3D<float> p = ((Point3DParameter*)sourceParam)->getVector();
	var value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex) : reference->getValue();

	if (currentFunctionId == equalsId)
	{
		if (value.size() < 3) return false;
		return p.x == (float)value[0] && p.y == (float)value[1] && p.z == (float)value[2];
	}
	else if (currentFunctionId == magnGreaterId)	return p.length() > (float)value;
	else if (currentFunctionId == magnLessId)		return p.length() < (float)value;
	else if (currentFunctionId == xGreaterId)	return p.x > (float)value;
	else if (currentFunctionId == xLessId)		return p.x < (float)value;
	else if (currentFunctionId == yGreaterId)	return p.y > (float)value;
	else if (currentFunctionId == yLessId)		return p.y < (float)value;
	else if (currentFunctionId == zGreaterId)	return p.z > (float)value;
	else if (currentFunctionId == zLessId)		return p.z < (float)value; 
	
	return false;
}
