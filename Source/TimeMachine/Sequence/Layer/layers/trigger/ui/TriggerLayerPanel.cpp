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
}

TriggerLayerPanel::~TriggerLayerPanel()
{
}
