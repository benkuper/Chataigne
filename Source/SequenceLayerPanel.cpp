/*
  ==============================================================================

    SequenceLayerPanel.cpp
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerPanel.h"

SequenceLayerPanel::SequenceLayerPanel(SequenceLayer * layer) :
	BaseItemUI<SequenceLayer>(layer)
{
	setSize(100, item->uiHeight->intValue());
}

SequenceLayerPanel::~SequenceLayerPanel()
{

}

void SequenceLayerPanel::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->uiHeight)
	{
		setSize(getWidth(), item->uiHeight->intValue());
	}
}
