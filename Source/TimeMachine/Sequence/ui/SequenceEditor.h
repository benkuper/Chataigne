/*
  ==============================================================================

    SequenceEditor.h
    Created: 28 Oct 2016 8:27:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEEDITOR_H_INCLUDED
#define SEQUENCEEDITOR_H_INCLUDED

#include "../Layer/ui/SequenceLayerPanelManagerUI.h"
#include "../Layer/ui/SequenceLayerTimelineManagerUI.h"
#include "SequenceTransportUI.h"
#include "SequenceTimelineNavigationUI.h"

class SequenceEditor :
	public Component,
	public Sequence::SequenceListener,
	public ScrollBar::Listener
{
public:
	SequenceEditor(Sequence * _sequence);
	virtual ~SequenceEditor();

	Sequence * sequence;
	WeakReference<Inspectable> sequenceRef;

	//ui
	Component panelContainer;
	Component timelineContainer;

	SequenceTimelineNavigationUI navigationUI;
	SequenceLayerPanelManagerUI panelManagerUI;
	SequenceLayerTimelineManagerUI timelineManagerUI;
	ScopedPointer<SequenceTransportUI> transportUI;

	//layout
	const float headerHeight = 50;
	float panelWidth;

	void paint(Graphics &g) override;
	void resized() override;
	
	// Inherited via Listener
	virtual void scrollBarMoved(ScrollBar * scrollBarThatHasMoved, double newRangeStart) override;

	void mouseWheelMove(const MouseEvent &e, const MouseWheelDetails &) override;
	bool keyPressed(const KeyPress &key) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceEditor)

		
};


#endif  // SEQUENCEEDITOR_H_INCLUDED
