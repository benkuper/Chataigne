/*
  ==============================================================================

    SequenceLayerTimelineManagerUI.h
    Created: 20 Nov 2016 2:18:18pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCELAYERTIMELINEMANAGERUI_H_INCLUDED
#define SEQUENCELAYERTIMELINEMANAGERUI_H_INCLUDED


#include "SequenceLayerTimeline.h"
#include "../SequenceLayerManager.h"

class SequenceLayerTimelineManagerUI :
	public BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerTimeline>
{
public:

	SequenceLayerTimelineManagerUI(SequenceLayerManager *_manager);
	~SequenceLayerTimelineManagerUI();

	SequenceLayerTimeline * createUIForItem(SequenceLayer * layer) override;

	void resized() override;

	void showMenuAndAddItem(bool, Point<int>) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerTimelineManagerUI)
};




#endif  // SEQUENCELAYERTIMELINEMANAGERUI_H_INCLUDED
