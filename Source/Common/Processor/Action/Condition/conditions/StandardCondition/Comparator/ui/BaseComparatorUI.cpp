/*
  ==============================================================================

    BaseComparatorUI.cpp
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseComparatorUI.h"

BaseComparatorUI::BaseComparatorUI(BaseComparator * _comparator) :
	comparator(_comparator),
	comparatorRef(_comparator)
{
	compareFuncUI.reset(comparator->compareFunction->createUI());

	if (comparator != nullptr && comparator->reference != nullptr) //null if comparator is trigger
	{
		if (comparator->refLink != nullptr)
		{
			refEditor.reset(new LinkableParameterEditor(comparator->refLink.get(), comparator->refLink->inputValueNames.size() > 0));
		}
		else
		{
			refEditor.reset((ControllableEditor*)comparator->reference->getEditor(false));
			((ControllableEditor*)refEditor.get())->setShowLabel(false);
		}

		if (comparator->reference->type != Controllable::TRIGGER)
		{
			Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
			p->addAsyncParameterListener(this);
		}

		addAndMakeVisible(compareFuncUI.get());
		addAndMakeVisible(refEditor.get());
	}

	setSize(100, 16); //init size
}

BaseComparatorUI::~BaseComparatorUI() 
{
	if (!comparatorRef.wasObjectDeleted() && comparator->reference != nullptr && comparator->reference->type != Controllable::TRIGGER)
	{
		Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
		p->removeAsyncParameterListener(this);
	}
}

void BaseComparatorUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2, 0);

	if (refEditor != nullptr)
	{
		compareFuncUI->setBounds(r.removeFromLeft(80).withHeight(16));
		r.removeFromLeft(2);

		refEditor->setBounds(r.withHeight(refEditor->getHeight()));
		setSize(getWidth(), refEditor->getBottom());
	} else
	{
		setSize(getWidth(), jmax<int>(compareFuncUI->getBottom(), 16));  //min 16pix to have the toggle
	}
}

void BaseComparatorUI::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.type == Parameter::ParameterEvent::CONTROLMODE_CHANGED) resized();
}
