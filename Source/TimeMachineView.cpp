/*
  ==============================================================================

    TimeMachineView.cpp
    Created: 28 Oct 2016 8:26:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "TimeMachineView.h"


TimeMachineView::TimeMachineView(const String &contentName, SequenceManager * _manager) :
	ShapeShifterContentComponent(contentName),
	manager(_manager),
	createSequenceBT("Create a sequence")
{
	contentIsFlexible = true;
	InspectableSelectionManager::getInstance()->addSelectionListener(this);
	SequenceManager::getInstance()->addBaseManagerListener(this);

	addAndMakeVisible(&createSequenceBT);
	createSequenceBT.addListener(this);
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
	else
	{
		createSequenceBT.setBounds(getLocalBounds().withSizeKeepingCentre(200, 100));
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
	
	createSequenceBT.setVisible(sequence == nullptr);
	resized();
}

void TimeMachineView::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::getInstance()->isEmpty()) return;
	
	ControllableContainer * cc = dynamic_cast<ControllableContainer *>(InspectableSelectionManager::getInstance()->currentInspectables[0]);
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

void TimeMachineView::buttonClicked(Button * b)
{
	if (b == &createSequenceBT)
	{
		SequenceManager::getInstance()->addItem();
	}
}
