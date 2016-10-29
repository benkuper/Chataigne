/*
  ==============================================================================

    SequenceLayerManagerUI.h
    Created: 28 Oct 2016 8:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERMANAGERUI_H_INCLUDED
#define SEQUENCELAYERMANAGERUI_H_INCLUDED


#include "BaseManagerUI.h"
#include "SequenceLayerUI.h"
#include "SequenceLayerManager.h"

class SequenceLayerManagerUI :
	public BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerUI>
{
public:

	SequenceLayerManagerUI(SequenceLayerManager *_manager);
	~SequenceLayerManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerManagerUI)
};




#endif  // SEQUENCELAYERMANAGERUI_H_INCLUDED
