/*
  ==============================================================================

    Point2DComparators.h
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../BaseComparator.h"

class Point2DComparator :
	public BaseComparator
{
public:
	Point2DComparator(Parameter * sourceParam);
	virtual ~Point2DComparator();

	const Identifier equalsId = "=";
	const Identifier distGreaterId = "d>";
	const Identifier distLessId = "d<";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";

	Point2DParameter * p2dRef;
	FloatParameter * valParam;

	virtual bool compare(Parameter * sourceParam) override;
};