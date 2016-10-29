/*
  ==============================================================================

    TimeMachineView.h
    Created: 28 Oct 2016 8:26:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TIMEMACHINEVIEW_H_INCLUDED
#define TIMEMACHINEVIEW_H_INCLUDED

#include "SequenceManager.h"
#include "ShapeShifterContent.h"
#include "Style.h"

class TimeMachineView :
	public ShapeShifterContent
{
public:
	TimeMachineView(SequenceManager * _manager);
	~TimeMachineView();

	void paint(Graphics &g) override;

	SequenceManager * manager;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeMachineView)
};


#endif  // TIMEMACHINEVIEW_H_INCLUDED
