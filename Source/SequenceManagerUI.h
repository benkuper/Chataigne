/*
  ==============================================================================

    SequenceManagerUI.h
    Created: 28 Oct 2016 8:13:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEMANAGERUI_H_INCLUDED
#define SEQUENCEMANAGERUI_H_INCLUDED


#include "SequenceManager.h"
#include "BaseManagerUI.h"
#include "SequenceUI.h"

class SequenceManagerUI :
	public BaseManagerUI<SequenceManager, Sequence, SequenceUI>
{
public:
	SequenceManagerUI(SequenceManager *_manager);
	~SequenceManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceManagerUI)
};





#endif  // SEQUENCEMANAGERUI_H_INCLUDED
