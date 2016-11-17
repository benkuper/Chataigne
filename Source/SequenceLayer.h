/*
  ==============================================================================

    SequenceLayer.h
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYER_H_INCLUDED
#define SEQUENCELAYER_H_INCLUDED


#include "BaseItem.h"
#include "Sequence.h"

class SequenceLayer :
	public BaseItem,
	public Sequence::SequenceListener
{
public:
	SequenceLayer();
	virtual ~SequenceLayer();

	Sequence * sequence;

	void setSequence(Sequence * _sequence);


	void sequenceCurrentTimeChanged(Sequence * _sequence, bool evaluateSkippedData);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayer)
};




#endif  // SEQUENCELAYER_H_INCLUDED
