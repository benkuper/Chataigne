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

	refUI = comparator->reference->createDefaultUI();
	addAndMakeVisible(refUI);

	setSize(10, 60); //init size
}

BaseComparatorUI::~BaseComparatorUI()
{
}

void BaseComparatorUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> hr = r.removeFromTop(16);
	alwaysDispatchUI->setBounds(hr.removeFromRight(60));
	hr.removeFromRight(2);
	compareFuncUI->setBounds(hr);

	r.removeFromTop(2);
	refUI->setBounds(r.removeFromTop(14));
}
