/*
  ==============================================================================

    Output.h
    Created: 28 Oct 2016 8:02:31pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "BaseItem.h"

class Output :
	public BaseItem
{
public:
	Output();
	virtual ~Output();


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Output)
};



#endif  // OUTPUT_H_INCLUDED
