/*
  ==============================================================================

    Point3DComparators.h
    Created: 2 Nov 2016 8:58:22pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Point3DComparator :
	public BaseComparator
{
public:
	Point3DComparator(Parameter * sourceParam, Multiplex* multiplex);
	virtual ~Point3DComparator();

	const Identifier equalsId = "=";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";
	const Identifier xGreaterId = "x>";
	const Identifier xLessId = "x<";
	const Identifier yGreaterId = "y>";
	const Identifier yLessId = "y<";
	const Identifier zGreaterId = "z>";
	const Identifier zLessId = "z<";
	Parameter* sourceParam;

	void onContainerParameterChanged(Parameter* p) override;
	void updateReferenceParam();


	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex = 0) override;
};