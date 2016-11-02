/*
  ==============================================================================

    FloatComparatorUI.cpp
    Created: 2 Nov 2016 11:52:09pm
    Author:  bkupe

  ==============================================================================
*/

#include "FloatComparatorUI.h"

FloatComparatorUI::FloatComparatorUI(BaseComparator * _comparator) :
	BaseComparatorUI(_comparator),
	fComparator((FloatComparator *)_comparator)
{
	refSlider = fComparator->ref->createSlider();
	addAndMakeVisible(refSlider);
}

FloatComparatorUI::~FloatComparatorUI()
{
}

void FloatComparatorUI::resized()
{
	BaseComparatorUI::resized();
	Rectangle<int> r = getLocalBounds();
	r.removeFromTop(18);
	r.removeFromTop(2);
	refSlider->setBounds(r.removeFromTop(14));
}
