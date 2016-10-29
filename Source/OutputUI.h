/*
  ==============================================================================

    OutputUI.h
    Created: 28 Oct 2016 8:22:07pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUTUI_H_INCLUDED
#define OUTPUTUI_H_INCLUDED

#include "BaseItemUI.h"
#include "Output.h"

class OutputUI :
	public BaseItemUI<Output>
{
public:
	OutputUI(Output * output);
	virtual ~OutputUI();


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputUI)
};


#endif  // OUTPUTUI_H_INCLUDED
