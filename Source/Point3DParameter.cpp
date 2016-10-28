/*
  ==============================================================================

    Point3DParameter.cpp
    Created: 30 Sep 2016 9:37:46am
    Author:  bkupe

  ==============================================================================
*/

#include "Point3DParameter.h"

Point3DParameter::Point3DParameter(const String & niceName, const String & description, bool enabled) :
	Parameter(POINT3D, niceName, description, 0, 0, 1, enabled)
{
}

void Point3DParameter::setPoint(Vector3D<float> _value)
{
	var d;
	d.append(_value.x);
	d.append(_value.y);
	d.append(_value.z);
	setValue(d);
}

void Point3DParameter::setValueInternal(var & _value)
{
	if (!_value.isArray()) return;
	x = _value[0];
	y = _value[1];
}

Vector3D<float> * Point3DParameter::getPoint() {
	return new Vector3D<float>(x, y,z);
}

bool Point3DParameter::checkValueIsTheSame(var newValue, var oldValue)
{
	if (!(newValue.isArray() && oldValue.isArray())) return false;
	return newValue[0] == oldValue[0] && newValue[1] == oldValue[1] && newValue[2] == oldValue[2];
}
