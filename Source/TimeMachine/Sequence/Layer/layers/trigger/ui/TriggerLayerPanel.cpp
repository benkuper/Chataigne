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
	lockAllBT.reset(layer->lockAll->createButtonUI());
	unlockAllBT.reset(layer->unlockAll->createButtonUI());
	addAndMakeVisible(lockAllBT.get());
	addAndMakeVisible(unlockAllBT.get());
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
