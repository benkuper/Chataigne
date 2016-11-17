/*
==============================================================================

Point3DParameter.cpp
Created: 30 Sep 2016 9:37:36am
Author:  bkupe

==============================================================================
*/

#include "Point3DParameter.h"
#include "TripleSliderUI.h"

Point3DParameter::Point3DParameter(const String & niceName, const String & description, bool enabled) :
	Parameter(POINT3D, niceName, description, 0, 0, 1, enabled),
	x(0), y(0),z(0),
	minX(0), maxX(1), minY(0), maxY(1), minZ(0),maxZ(1)
{

	value = var();
	value.append(0);
	value.append(0);
	value.append(0);

	//hideInEditor = true;
	argumentsDescription = "float, float, float";
}

void Point3DParameter::setVector(Vector3D<float> _value)
{
	setVector(_value.x, _value.y,_value.z);
}

void Point3DParameter::setVector(float _x, float _y, float _z)
{
	var d;

	d.append(jlimit<float>(minX, maxX, _x));
	d.append(jlimit<float>(minY, maxY, _y));
	d.append(jlimit<float>(minZ, maxZ, _z));

	if (d[0] == value[0] && d[1] == value[1] && d[2] == value[2]) return;
	setValue(d);
}

void Point3DParameter::setValueInternal(var & _value)
{
	if (!_value.isArray()) return;
	if (autoAdaptRange)
	{
		bool hasChanged = false;
		if ((float)_value[0] < (float)minX) {
			minX = _value[0];
			hasChanged = true;
		} else if ((float)_value[0] >(float)maxX)
		{
			maxX = _value[0];
			hasChanged = true;
		}

		if ((float)_value[1] < (float)minY) {
			minY = _value[1];
			hasChanged = true;
		} else if ((float)_value[1] >(float)maxY)
		{
			maxY = _value[1];
			hasChanged = true;
		}

		if ((float)_value[2] < (float)minZ) {
			minZ = _value[2];
			hasChanged = true;
		} else if ((float)_value[2] >(float)maxZ)
		{
			maxZ = _value[2];
			hasChanged = true;
		}

		if (hasChanged)
		{
			listeners.call(&Listener::parameterRangeChanged, this);
			var arr;
			arr.append(minX); arr.append(maxX); arr.append(minY); arr.append(maxY); arr.append(minZ);  arr.append(maxZ);
			queuedNotifier.addMessage(new ParamWithValue(this, arr));
		}
	}



	x = jlimit<float>(minX, maxX, _value[0]);
	y = jlimit<float>(minY, maxY, _value[1]);
	z = jlimit<float>(minZ, maxZ, _value[2]);

	value = var();
	value.append(x);
	value.append(y);
	value.append(z);
}

void Point3DParameter::setBounds(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ)
{
	minX = _minX;
	minY = _minY;
	minZ = _minZ;
	maxX = _maxX;
	maxY = _maxY;
	maxZ = _maxZ;
	listeners.call(&Listener::parameterRangeChanged, this);
	var arr;
	arr.append(minX); arr.append(maxX); arr.append(minY); arr.append(maxY); arr.append(minZ); arr.append(maxZ);
	queuedNotifier.addMessage(new ParamWithValue(this, arr));
}

Vector3D<float> Point3DParameter::getVector() {
	return Vector3D<float>(x, y,z);
}

bool Point3DParameter::checkValueIsTheSame(var newValue, var oldValue)
{
	if (!(newValue.isArray() && oldValue.isArray())) return false;

	return newValue[0] == oldValue[0] && newValue[1] == oldValue[1];
}

ControllableUI * Point3DParameter::createDefaultUI(Controllable * targetControllable)
{
	return new TripleSliderUI(targetControllable != nullptr ? (Point3DParameter *)targetControllable : this);
}
