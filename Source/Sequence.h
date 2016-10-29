/*
  ==============================================================================

    Sequence.h
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "BaseItem.h"

class Sequence :
	public BaseItem
{
public:
	Sequence();
	virtual ~Sequence();


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequence)
};




#endif  // SEQUENCE_H_INCLUDED
