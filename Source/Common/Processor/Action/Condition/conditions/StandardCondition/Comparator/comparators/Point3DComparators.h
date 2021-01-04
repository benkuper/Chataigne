/*
  ==============================================================================

    Point3DComparators.h
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class Point3DComparator :
	public BaseComparator
{
public:
	Point3DComparator(Parameter * sourceParam);
	virtual ~Point3DComparator();

	const Identifier equalsId = "=";
	const Identifier distGreaterId = "d>";
	const Identifier distLessId = "d<";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";

	Point3DParameter * p3dRef;
	FloatParameter * valParam;

	virtual bool compare(Parameter * sourceParam) override;
};