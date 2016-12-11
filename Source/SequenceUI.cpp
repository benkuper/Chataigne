/*
  ==============================================================================

    SequenceUI.cpp
    Created: 28 Oct 2016 8:13:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceUI.h"

SequenceUI::SequenceUI(Sequence * sequence) :
	BaseItemUI<Sequence>(sequence)
{
	
	timeUI = item->currentTime->createSlider();
	timeUI->setForceFeedbackOnly(true);
	timeUI->showLabel = false;
	timeUI->showValue = false;
	timeUI->bgColor = BG_COLOR.darker(.1f);
	addAndMakeVisible(timeUI);
	setSize(100, 30);

	item->addAsyncContainerListener(this);
}

SequenceUI::~SequenceUI()
{
	item->removeAsyncContainerListener(this);
}

void SequenceUI::resized()
{
	BaseItemUI::resized();
	Rectangle<int> r = getLocalBounds();
	r.removeFromTop(headerHeight + headerGap);
	r.reduce(2, 2);
	timeUI->setBounds(r);
}

void SequenceUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->isPlaying)
	{
		if (item->isPlaying->boolValue()) timeUI->setFrontColor(HIGHLIGHT_COLOR);
		else timeUI->resetFrontColor();
	}
}
