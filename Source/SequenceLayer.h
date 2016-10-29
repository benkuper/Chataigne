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

class SequenceLayer :
	public BaseItem
{
public:
	SequenceLayer();
	virtual ~SequenceLayer();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayer)
};




#endif  // SEQUENCELAYER_H_INCLUDED
