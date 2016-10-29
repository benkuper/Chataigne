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
	x(0), y(0),
	minX(0),maxX(1),minY(0),maxY(1)
{

	value = var();
	value.append(0);
	value.append(0);

	//hideInEditor = true;
}

void Point2DParameter::setPoint(Point<float> _value)
{
	setPoint(_value.x, _value.y);
}

void Point2DParameter::setPoint(float _x, float _y)
{
	var d;
	d.append(jlimit<float>(minX,maxX,_x));
	d.append(jlimit<float>(minY, maxY, _y));

	if (d[0] == value[0] && d[1] == value[1]) return;
	setValue(d);
}

void Point2DParameter::setValueInternal(var & _value)
{
	if (!_value.isArray()) return;
	x = jlimit<float>(minX, maxX, _value[0]);
	y = jlimit<float>(minY, maxY,_value[1]);

	value = var();
	value.append(x);
	value.append(y);
}

void Point2DParameter::setBounds(float _minX, float _minY, float _maxX, float _maxY)
{
	minX = _minX;
	minY = _minY;
	maxX = _maxX;
	maxY = _maxY;
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
