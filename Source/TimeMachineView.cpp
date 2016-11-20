/*
  ==============================================================================

    TimeMachineView.cpp
    Created: 28 Oct 2016 8:26:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "TimeMachineView.h"


TimeMachineView::TimeMachineView(SequenceManager * _manager) :
	ShapeShifterContentComponent("Time Machine"),
	manager(_manager) 
{
	contentIsFlexible = true;

	
	Inspector::getInstance()->addInspectorListener(this);
}

TimeMachineView::~TimeMachineView()
{
	if(Inspector::getInstanceWithoutCreating()) Inspector::getInstance()->removeInspectorListener(this);
}

void TimeMachineView::paint(Graphics & g)
{
	g.fillAll(BG_COLOR.darker(.3f));
}

void TimeMachineView::resized()
{
	if (editor != nullptr)
	{
		editor->setBounds(getLocalBounds());
	}
}

void TimeMachineView::setSequence(Sequence * sequence)
{
	if (editor != nullptr && editor->sequence == sequence) return;

	if (editor != nullptr)
	{
		removeChildComponent(editor);
		editor = nullptr;
	}

	if (sequence != nullptr)
	{
		editor = new SequenceEditor(sequence);
		addAndMakeVisible(editor);
	}
	resized();
}

void TimeMachineView::currentInspectableChanged(Inspector * i)
{
	if (dynamic_cast<Sequence *>(i->currentInspectable) != nullptr)
	{
		setSequence(static_cast<Sequence *>(i->currentInspectable));
	} else if (dynamic_cast<SequenceLayer *>(i->currentInspectable) != nullptr)
	{
		setSequence(static_cast<SequenceLayer *>(i->currentInspectable)->sequence);
	}
}
