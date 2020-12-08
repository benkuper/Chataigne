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
	alwaysTriggerUI.reset(comparator->alwaysTrigger->createToggle());
	toggleModeUI.reset(comparator->toggleMode->createToggle(ImageCache::getFromMemory(BinaryData::toggle_png, BinaryData::toggle_pngSize)));
	
	addAndMakeVisible(toggleModeUI.get());

	if (!comparator.wasObjectDeleted() && comparator->reference != nullptr) //null if comparator is trigger
	{
		refEditor.reset((ControllableEditor*)comparator->reference->getEditor(false));
		refEditor->setShowLabel(false);
		if (comparator->reference->type != Controllable::TRIGGER)
		{
			Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
			p->addAsyncParameterListener(this);
		}

		addAndMakeVisible(alwaysTriggerUI.get());
		addAndMakeVisible(compareFuncUI.get());
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
	
	Rectangle<int> r = getLocalBounds().reduced(2, 0);
		
	toggleModeUI->setBounds(r.removeFromLeft(16).withHeight(16));
	r.removeFromLeft(2);

	if (refEditor != nullptr)
	{
		compareFuncUI->setBounds(r.removeFromLeft(80).withHeight(16));
		r.removeFromLeft(2);

		alwaysTriggerUI->setBounds(r.removeFromRight(95).reduced(0, 1));
		r.removeFromRight(4);

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
