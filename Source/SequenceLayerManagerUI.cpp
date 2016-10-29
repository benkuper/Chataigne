/*
  ==============================================================================

    SequenceLayerManagerUI.cpp
    Created: 28 Oct 2016 8:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerManagerUI.h"


SequenceLayerManagerUI::SequenceLayerManagerUI(SequenceLayerManager * _manager) :
	BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerUI>("SequenceLayers", _manager)
{
	addItemText = "Add Layer";
}

SequenceLayerManagerUI::~SequenceLayerManagerUI()
{
}
