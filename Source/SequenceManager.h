/*
  ==============================================================================

    SequenceManager.h
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEMANAGER_H_INCLUDED
#define SEQUENCEMANAGER_H_INCLUDED


#include "BaseManager.h"
#include "Sequence.h"

class SequenceManager :
	public BaseManager<Sequence>
{
public:
	juce_DeclareSingleton(SequenceManager, false)

		SequenceManager();
	~SequenceManager();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceManager)
};





#endif  // SEQUENCEMANAGER_H_INCLUDED
