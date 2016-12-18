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
	resizer.setBorderThickness(BorderSize<int>(0, 0, 6, 0));
	addAndMakeVisible(&resizer);
	resizer.toBack();

	setSize(100, item->uiHeight->intValue());
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
