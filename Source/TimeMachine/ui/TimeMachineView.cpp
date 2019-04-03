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
	manager(_manager)
{
	contentIsFlexible = true;
	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);
	SequenceManager::getInstance()->addBaseManagerListener(this);
	helpID = "SequenceEditor";


	inspectablesSelectionChanged(); //force selecting 


}

TimeMachineView::~TimeMachineView()
{
	SequenceManager::getInstance()->removeBaseManagerListener(this);
	if(InspectableSelectionManager::mainSelectionManager) InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
}

void TimeMachineView::paint(Graphics & g)
{
	g.fillAll(BG_COLOR.darker(.3f));
	if (editor == nullptr)
	{
		g.setFont(20);
		g.setColour(BG_COLOR.brighter(.1f));
		g.drawFittedText("Create a sequence from the Sequences Panel to edit this sequence here.", getLocalBounds(), Justification::centred, 3);
	}
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

void TimeMachineView::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->isEmpty()) return;
	
	ControllableContainer * cc = InspectableSelectionManager::mainSelectionManager->getInspectableAs<ControllableContainer>();
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
