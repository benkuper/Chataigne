/*
  ==============================================================================

    FloatComparatorUI.h
    Created: 2 Nov 2016 11:52:09pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef FLOATCOMPARATORUI_H_INCLUDED
#define FLOATCOMPARATORUI_H_INCLUDED

#include "BaseComparatorUI.h"
#include "NumberComparators.h"
#include "FloatSliderUI.h"

class FloatComparatorUI :
	public BaseComparatorUI
{
public :
	FloatComparatorUI(BaseComparator * _comparator);
	virtual ~FloatComparatorUI();

	FloatComparator * fComparator;
	ScopedPointer<FloatSliderUI> refSlider;

	void resized() override;
};


#endif  // FLOATCOMPARATORUI_H_INCLUDED
