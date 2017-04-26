/*
==============================================================================

Point3DParameter.h
Created: 30 Sep 2016 9:37:36am
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

	float minX, maxX,minY,maxY, minZ, maxZ;
	float x, y, z;
	void setVector(Vector3D<float> value);
	void setVector(float x, float y, float z);
	void setValueInternal(var & _value) override;

	void setBounds(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ);

	Vector3D<float> getVector();

	bool checkValueIsTheSame(var newValue, var oldValue) override;

	ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

	static Point3DParameter * create() { return new Point3DParameter("New Point3D Parameter", "", 0); }
	virtual String getTypeString() const override { return "Point 3D"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Point3DParameter)
};


#endif  // Point3DPARAMETER_H_INCLUDED
