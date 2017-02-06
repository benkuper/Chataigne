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
	InspectableSelectionManager::getInstance()->addSelectionListener(this);
	SequenceManager::getInstance()->addBaseManagerListener(this);
}

TimeMachineView::~TimeMachineView()
{
	SequenceManager::getInstance()->removeBaseManagerListener(this);
	if(InspectableSelectionManager::getInstanceWithoutCreating()) InspectableSelectionManager::getInstance()->removeSelectionListener(this);
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

void TimeMachineView::currentInspectableSelectionChanged(Inspectable *, Inspectable * i)
{
	ControllableContainer * cc = dynamic_cast<ControllableContainer *>(i);	
	if (cc == nullptr) return;

	Sequence * s = nullptr;
	while (cc != nullptr)
	{
		s = dynamic_cast<Sequence *>(cc);
		if (s != nullptr) break;
		cc = cc->parentContainer;
	}

	if(s != nullptr) setSequence(s);
}


void TimeMachineView::itemRemoved(Sequence *s)
{
	if (editor != nullptr && s == editor->sequence) setSequence(nullptr);
}
