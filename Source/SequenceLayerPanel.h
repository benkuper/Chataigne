/*
  ==============================================================================

    SequenceLayerPanel.h
    Created: 17 Nov 2016 7:59:08pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCELAYERPANEL_H_INCLUDED
#define SEQUENCELAYERPANEL_H_INCLUDED


#include "BaseItemUI.h"
#include "SequenceLayer.h"

class SequenceLayerPanel :
	public BaseItemUI<SequenceLayer>
{
public:
	SequenceLayerPanel(SequenceLayer *);
	virtual ~SequenceLayerPanel();

	void resized() override;

	ResizableBorderComponent resizer;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerPanel)
};



#endif  // SEQUENCELAYERPANEL_H_INCLUDED
