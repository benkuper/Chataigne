/*
  ==============================================================================

    BaseComparatorUI.cpp
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

BaseComparatorUI::BaseComparatorUI(BaseComparator * _comparator) :
	comparator(_comparator),
	comparatorRef(_comparator)
{
	if (comparator != nullptr)
	{
		compareFuncUI.reset(comparator->compareFunction->createUI());
		addAndMakeVisible(compareFuncUI.get());

		comparator->addAsyncComparatorListener(this);
	}

	updateReferenceUI();

	setSize(100, 16); //init size
}

BaseComparatorUI::~BaseComparatorUI() 
{
	if (!comparatorRef.wasObjectDeleted() && comparator->reference != nullptr && comparator->reference->type != Controllable::TRIGGER)
	{
		Parameter * p = dynamic_cast<Parameter *>(comparator->reference);
		p->removeAsyncParameterListener(this);

		comparator->removeAsyncComparatorListener(this);
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

void BaseComparatorUI::updateReferenceUI()
{
	if (refEditor != nullptr)
	{
		removeChildComponent(refEditor.get());
		refEditor.reset();
	}

	if (comparator != nullptr && comparator->reference != nullptr) //null if comparator is trigger
	{
		if (comparator->refLink != nullptr)
		{
			refEditor.reset(new LinkableParameterEditor(comparator->refLink.get(), comparator->refLink->inputValueNames.size() > 0));
			((LinkableParameterEditor*)refEditor.get())->paramEditor->setShowLabel(false);
		}
		else
		{
			refEditor.reset((ControllableEditor*)comparator->reference->getEditor(false));
			((ControllableEditor*)refEditor.get())->setShowLabel(false);
		}

		if (comparator->reference->type != Controllable::TRIGGER)
		{
			Parameter* p = dynamic_cast<Parameter*>(comparator->reference);
			p->addAsyncParameterListener(this);
		}

		addAndMakeVisible(refEditor.get());
		resized();
	}
}

void BaseComparatorUI::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.type == Parameter::ParameterEvent::CONTROLMODE_CHANGED) resized();
}

void BaseComparatorUI::newMessage(const BaseComparator::ComparatorEvent& e)
{
	if (e.type == BaseComparator::ComparatorEvent::REFERENCE_CHANGED)
	{
		updateReferenceUI();
	}
}
