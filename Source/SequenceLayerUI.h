/*
  ==============================================================================

    SequenceLayerUI.h
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERUI_H_INCLUDED
#define SEQUENCELAYERUI_H_INCLUDED


#include "BaseItemUI.h"
#include "SequenceLayer.h"

class SequenceLayerUI :
	public BaseItemUI<SequenceLayer>
{
public:
	SequenceLayerUI(SequenceLayer *);
	virtual ~SequenceLayerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerUI)
};





#endif  // SEQUENCELAYERUI_H_INCLUDED
