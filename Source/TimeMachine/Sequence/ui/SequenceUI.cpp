/*
  ==============================================================================

    SequenceUI.cpp
    Created: 28 Oct 2016 8:13:34pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceUI.h"
#include "UI/ChataigneAssetManager.h"

SequenceUI::SequenceUI(Sequence * sequence) :
	BaseItemUI<Sequence>(sequence)
{
	minContentHeight = 20; //To fix : need to pass in constructor of BaseItemUI

	togglePlayUI = item->togglePlayTrigger->createImageUI(ChataigneAssetManager::getInstance()->getPlayImage());
	stopUI = item->stopTrigger->createImageUI(ChataigneAssetManager::getInstance()->getStopImage());
	addAndMakeVisible(togglePlayUI);
	addAndMakeVisible(stopUI);


	timeUI = item->currentTime->createSlider();
	timeUI->showLabel = false;
	timeUI->showValue = false;
	timeUI->bgColor = BG_COLOR.darker(.1f);
	addAndMakeVisible(timeUI);

	bgColor = item->isBeingEdited ? BLUE_COLOR.darker() : BG_COLOR.brighter(.1f); 
	
	item->addAsyncContainerListener(this);
	item->addAsyncSequenceListener(this);
}

SequenceUI::~SequenceUI()
{
	item->removeAsyncContainerListener(this);
	if (!Engine::mainEngine->isClearing && !inspectable.wasObjectDeleted()) item->removeAsyncSequenceListener(this);
}

void SequenceUI::resizedInternalHeader(Rectangle<int>& r)
{
	stopUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
	r.removeFromRight(2);
	togglePlayUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
	r.removeFromRight(2);

	BaseItemUI::resizedInternalHeader(r);

}

void SequenceUI::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(8);
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

void SequenceUI::newMessage(const Sequence::SequenceEvent & e)
{
	switch (e.type)
	{
	case Sequence::SequenceEvent::EDITING_STATE_CHANGED:
	{
		bgColor = item->isBeingEdited ? BLUE_COLOR.darker() : BG_COLOR.brighter(.1f);
		repaint();
	}
	break;
	}
}
