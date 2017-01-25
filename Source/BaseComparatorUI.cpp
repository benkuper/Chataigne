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
	//alwaysDispatchUI = comparator->alwaysDispatch->createToggle();
	
	addAndMakeVisible(compareFuncUI);
	//addAndMakeVisible(alwaysDispatchUI);

	refEditor = (ControllableEditor *)comparator->reference->getEditor(false);
	refEditor->setShowLabel(false);
	addAndMakeVisible(refEditor);

	setSize(100, 18); //init size
}

BaseComparatorUI::~BaseComparatorUI()
{
}

void BaseComparatorUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2,0);
	compareFuncUI->setBounds(r.removeFromLeft(80));
	r.removeFromLeft(2);
	refEditor->setBounds(r);
}
