/*
  ==============================================================================

    SequenceLayerPanelManagerUI.h
    Created: 28 Oct 2016 8:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERPANELMANAGERUI_H_INCLUDED
#define SEQUENCELAYERPANELMANAGERUI_H_INCLUDED



#include "SequenceLayerPanel.h"
#include "SequenceLayerManager.h"

class SequenceLayerPanelManagerUI :
	public BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerPanel>
{
public:

	SequenceLayerPanelManagerUI(SequenceLayerManager *_manager);
	~SequenceLayerPanelManagerUI();


	void showMenuAndAddItem(bool isFromAddButton, Point<int>) override;

	SequenceLayerPanel * createUIForItem(SequenceLayer * layer) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerPanelManagerUI)
};




#endif  // SEQUENCELAYERMANAGERUI_H_INCLUDED
