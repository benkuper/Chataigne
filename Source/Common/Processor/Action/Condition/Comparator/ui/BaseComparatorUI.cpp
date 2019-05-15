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
	compareFuncUI.reset(comparator->compareFunction->createUI());
	//alwaysDispatchUI = comparator->alwaysDispatch->createToggle();
	
	addAndMakeVisible(compareFuncUI.get());
	//addAndMakeVisible(alwaysDispatchUI);

	if (!comparator.wasObjectDeleted() && comparator->reference != nullptr) //null if comparator is trigger
	{
		refEditor.reset((ControllableEditor*)comparator->reference->getEditor(false));
		refEditor->setShowLabel(false);
		if (comparator->reference->type != Controllable::TRIGGER)
		{
			Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
			p->addAsyncParameterListener(this);
		}

		addAndMakeVisible(refEditor.get());
	}

	setSize(100, 16); //init size
}

BaseComparatorUI::~BaseComparatorUI() 
{
	if (!comparator.wasObjectDeleted() && comparator->reference != nullptr && comparator->reference->type != Controllable::TRIGGER)
	{
		Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
		p->removeAsyncParameterListener(this);
	}
}

void BaseComparatorUI::resized()
{
	if (refEditor != nullptr)
	{
		Rectangle<int> r = getLocalBounds().reduced(2, 0);
		compareFuncUI->setBounds(r.removeFromLeft(80).withHeight(16)); 
		r.removeFromLeft(2);
		refEditor->setBounds(r.withHeight(refEditor->getHeight()));
		setSize(getWidth(), refEditor->getBottom());
	} else
	{
		setSize(getWidth(), compareFuncUI->getBottom()); 
	}

}

void BaseComparatorUI::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.type == Parameter::ParameterEvent::CONTROLMODE_CHANGED) resized();
}
