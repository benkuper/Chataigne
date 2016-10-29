/*
  ==============================================================================

    OutputManagerUI.h
    Created: 28 Oct 2016 8:22:11pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUTMANAGERUI_H_INCLUDED
#define OUTPUTMANAGERUI_H_INCLUDED

#include "OutputManager.h"
#include "BaseManagerShapeShifterUI.h"
#include "OutputUI.h"

class OutputManagerUI :
	public BaseManagerShapeShifterUI<OutputManager, Output, OutputUI>
{
public:
	OutputManagerUI(OutputManager *_manager);
	~OutputManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputManagerUI)
};



#endif  // OUTPUTMANAGERUI_H_INCLUDED
