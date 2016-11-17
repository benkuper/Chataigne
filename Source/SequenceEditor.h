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

	Sequence * sequence;
	SequenceLayerManagerUI lmui;
};


#endif  // SEQUENCEEDITOR_H_INCLUDED
