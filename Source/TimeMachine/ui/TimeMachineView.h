/*
  ==============================================================================

    TimeMachineView.h
    Created: 28 Oct 2016 8:26:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TIMEMACHINEVIEW_H_INCLUDED
#define TIMEMACHINEVIEW_H_INCLUDED

#include "../SequenceManager.h"
#include "../Sequence/ui/SequenceEditor.h"

class TimeMachineView :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public SequenceManager::Listener
{
public:
	
	TimeMachineView(const String &contentName, SequenceManager * _manager = nullptr);
	~TimeMachineView();

	SequenceManager * manager;
	std::unique_ptr<SequenceEditor> editor;
	
	void paint(Graphics &g) override;
	void resized() override;

	void setSequence(Sequence * sequence);
	void inspectablesSelectionChanged() override;

	void itemRemoved(Sequence *) override;

	static TimeMachineView * create(const String &contentName) { return new TimeMachineView(contentName, SequenceManager::getInstance()); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeMachineView)
};


#endif  // TIMEMACHINEVIEW_H_INCLUDED
