/*
  ==============================================================================

    Point2DComparators.h
    Created: 2 Nov 2016 8:58:14pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Point2DComparator :
	public BaseComparator
{
public:
	Point2DComparator(Parameter * sourceParam, Multiplex* multiplex);
	virtual ~Point2DComparator();

	const Identifier equalsId = "=";
	const Identifier magnGreaterId = "m>";
	const Identifier magnLessId = "m<";
	const Identifier xGreaterId = "x>";
	const Identifier xLessId = "x<";
	const Identifier yGreaterId = "y>";
	const Identifier yLessId = "y<";

	Parameter* sourceParam;

	void onContainerParameterChanged(Parameter* p) override;
	void updateReferenceParam();

	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex = 0) override;
};