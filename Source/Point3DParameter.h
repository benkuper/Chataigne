/*
  ==============================================================================

    Point3DParameter.h
    Created: 30 Sep 2016 9:37:46am
    Author:  bkupe

  ==============================================================================
*/

#ifndef POINT3DPARAMETER_H_INCLUDED
#define POINT3DPARAMETER_H_INCLUDED



#include "Parameter.h"

class Point3DParameter : public Parameter
{
public:
	Point3DParameter(const String &niceName, const String &description, bool enabled = true);
	~Point3DParameter() {}

	float x, y, z;
	void setPoint(Vector3D<float> value);
	void setValueInternal(var & _value) override;

	Vector3D<float> * getPoint();

	bool checkValueIsTheSame(var newValue, var oldValue) override;

	ControllableUI * createDefaultUI(Controllable * /*targetControllable*/ = nullptr) override { return nullptr; }
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Point3DParameter)
};


#endif  // POINT3DPARAMETER_H_INCLUDED
