/*
  ==============================================================================

    TriggerLayerPanel.cpp
    Created: 20 Nov 2016 3:07:50pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TriggerLayerPanel.h"

TriggerLayerPanel::TriggerLayerPanel(TriggerLayer * layer) :
	SequenceLayerPanel(layer),
	triggerLayer(layer)
{

	bgColor = ACTION_COLOR.withSaturation(.2f).darker(1);

	lockAllBT = layer->lockAll->createButtonUI();
	unlockAllBT = layer->unlockAll->createButtonUI();
	addAndMakeVisible(lockAllBT);
	addAndMakeVisible(unlockAllBT);
}

TriggerLayerPanel::~TriggerLayerPanel()
{
}

void TriggerLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalContent(r);

	Rectangle<int> btr = r.removeFromTop(12);

	lockAllBT->setBounds(btr.removeFromLeft(60));
	btr.removeFromLeft(10);
	unlockAllBT->setBounds(btr.removeFromLeft(60));
}
