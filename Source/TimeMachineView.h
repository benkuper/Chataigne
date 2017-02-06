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
#include "SequenceEditor.h"
#include "InspectableSelectionManager.h"

class TimeMachineView :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public SequenceManager::Listener
{
public:
	
	TimeMachineView(SequenceManager * _manager = nullptr);
	~TimeMachineView();

	SequenceManager * manager;

	ScopedPointer<SequenceEditor> editor;

	
	void paint(Graphics &g) override;
	void resized() override;

	void setSequence(Sequence * sequence);
	void currentInspectableSelectionChanged(Inspectable *, Inspectable *) override;

	void itemRemoved(Sequence *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeMachineView)
};


#endif  // TIMEMACHINEVIEW_H_INCLUDED
