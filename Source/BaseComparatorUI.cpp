/*
  ==============================================================================

    BaseComparatorUI.cpp
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseComparatorUI.h"

BaseComparatorUI::BaseComparatorUI(BaseComparator * _comparator) :
	comparator(_comparator)
{
	compareFuncUI = comparator->compareFunction->createUI();
	alwaysDispatchUI = comparator->alwaysDispatch->createToggle();
	
	addAndMakeVisible(compareFuncUI);
	addAndMakeVisible(alwaysDispatchUI);

	setSize(10, 60); //init size
}

BaseComparatorUI::~BaseComparatorUI()
{
}

void BaseComparatorUI::resized()
{
	Rectangle<int> r = getLocalBounds().removeFromTop(16);
	alwaysDispatchUI->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
	compareFuncUI->setBounds(r);
}
