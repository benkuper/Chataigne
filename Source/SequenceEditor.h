/*
  ==============================================================================

    SequenceEditor.h
    Created: 28 Oct 2016 8:27:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEEDITOR_H_INCLUDED
#define SEQUENCEEDITOR_H_INCLUDED

#include "SequenceLayerManagerUI.h"

class SequenceEditor :
	public Component,
	public Sequence::SequenceListener
{
public:
	SequenceEditor(Sequence * _sequence);
	virtual ~SequenceEditor();


	void paint(Graphics &g) override;

	Sequence * sequence;
	SequenceLayerManagerUI lmui;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceEditor)
};


#endif  // SEQUENCEEDITOR_H_INCLUDED
