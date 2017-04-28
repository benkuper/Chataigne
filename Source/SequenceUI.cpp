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
	minContentHeight = 5;

	timeUI = item->currentTime->createSlider();
	timeUI->setForceFeedbackOnly(true);
	timeUI->showLabel = false;
	timeUI->showValue = false;
	timeUI->bgColor = BG_COLOR.darker(.1f);
	addAndMakeVisible(timeUI);

	item->addAsyncContainerListener(this);
}

SequenceUI::~SequenceUI()
{
	item->removeAsyncContainerListener(this);
}

void SequenceUI::resizedInternalContent(Rectangle<int>& r)
{
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
