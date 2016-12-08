/*
  ==============================================================================

    SequenceLayerPanel.cpp
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerPanel.h"

SequenceLayerPanel::SequenceLayerPanel(SequenceLayer * layer) :
	BaseItemUI<SequenceLayer>(layer),
	resizer(this,nullptr)
{
	setSize(100, item->uiHeight->intValue());
	addAndMakeVisible(&resizer);
}

SequenceLayerPanel::~SequenceLayerPanel()
{

}

void SequenceLayerPanel::resized()
{
	BaseItemUI::resized();
	item->uiHeight->setValue(getHeight());
	resizer.setBounds(getLocalBounds());
}

void SequenceLayerPanel::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->uiHeight)
	{
		setSize(getWidth(), item->uiHeight->intValue());
	}

}
