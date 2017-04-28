/*
  ==============================================================================

    Point2DParameter.cpp
    Created: 30 Sep 2016 9:37:36am
    Author:  bkupe

  ==============================================================================
*/

#include "Point2DParameter.h"
#include "DoubleSliderUI.h"

Point2DParameter::Point2DParameter(const String & niceName, const String & description, bool enabled) :
	Parameter(POINT2D, niceName, description, 0, 0, 1, enabled),
	x(0), y(0)
{

	value = var();
	value.append(0);
	value.append(0);

	minimumValue = var();
	minimumValue.append(0);
	minimumValue.append(0);

	maximumValue = var();
	maximumValue.append(1);
	maximumValue.append(1);

	//hideInEditor = true;
	argumentsDescription = "float, float";
}

void Point2DParameter::setPoint(Point<float> _value)
{
	setPoint(_value.x, _value.y);
}

void Point2DParameter::setPoint(float _x, float _y)
{
	var d;
	d.append(jlimit<float>(minimumValue[0],maximumValue[0],_x));
	d.append(jlimit<float>(minimumValue[1],maximumValue[1], _y));

	if (d[0] == value[0] && d[1] == value[1]) return;
	setValue(d);
}

void Point2DParameter::setValueInternal(var & _value)
{
	if (!_value.isArray()) return;

	if (autoAdaptRange)
	{
		bool hasChanged = false;
		for (int i = 0; i < 3; i++)
		{
			if ((float)_value[i] < (float)minimumValue[i]) {
				minimumValue[i] = _value[i];
				hasChanged = true;
			} else if ((float)_value[i] > (float)maximumValue[i])
			{
				maximumValue[i] = _value[i];
				hasChanged = true;
			}
		}

		if(hasChanged) setRange(minimumValue, maximumValue);
	}

	x = jlimit<float>(minimumValue[0], maximumValue[0], _value[0]);
	y = jlimit<float>(minimumValue[1], maximumValue[1],_value[1]);

	value = var();
	value.append(x);
	value.append(y);
}

void Point2DParameter::setBounds(float _minX, float _minY, float _maxX, float _maxY)
{
	var minRange;
	var maxRange;
	minRange.append(_minX);
	minRange.append(_minY);
	maxRange.append(_maxX);
	maxRange.append(_maxY);
	setRange(minRange, maxRange, true);
}

Point<float> Point2DParameter::getPoint() {
	return Point<float>(x, y); 
}

bool Point2DParameter::checkValueIsTheSame(var newValue, var oldValue)
{
	if (!(newValue.isArray() && oldValue.isArray())) return false;

	return newValue[0] == oldValue[0] && newValue[1] == oldValue[1];
}

ControllableUI * Point2DParameter::createDefaultUI(Controllable * targetControllable) 
{ 
	return new DoubleSliderUI(targetControllable != nullptr?(Point2DParameter *)targetControllable:this);  
}
