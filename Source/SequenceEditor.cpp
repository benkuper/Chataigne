/*
  ==============================================================================

    SequenceEditor.cpp
    Created: 28 Oct 2016 8:27:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceEditor.h"

SequenceEditor::SequenceEditor(Sequence * _sequence) :
	lmui(_sequence->layerManager),
	sequence(_sequence)
{
	
}

SequenceEditor::~SequenceEditor()
{
}

void SequenceEditor::paint(Graphics & g)
{
	g.fillAll(Colours::red);
}
