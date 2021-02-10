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
	Point3DComparator(Parameter * sourceParam, Multiplex* multiplex);
	virtual ~Point3DComparator();

	const Identifier equalsId = "=";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";

	Parameter* sourceParam;

	void onContainerParameterChanged(Parameter* p) override;
	void updateReferenceParam();


	virtual bool compare(Parameter* sourceParam, int multiplexIndex = 0) override;
};