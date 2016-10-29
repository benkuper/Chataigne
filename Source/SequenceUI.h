/*
  ==============================================================================

    SequenceUI.h
    Created: 28 Oct 2016 8:13:34pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEUI_H_INCLUDED
#define SEQUENCEUI_H_INCLUDED

#include "BaseItemUI.h"
#include "Sequence.h"

class SequenceUI :
	public BaseItemUI<Sequence>
{
public:
	SequenceUI(Sequence * output);
	virtual ~SequenceUI();

	JUCE_DECLARE_NON_COPYABLE(SequenceUI)
};






#endif  // SEQUENCEUI_H_INCLUDED
