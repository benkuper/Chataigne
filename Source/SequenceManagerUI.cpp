/*
  ==============================================================================

    SequenceManagerUI.cpp
    Created: 28 Oct 2016 8:13:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceManagerUI.h"

SequenceManagerUI::SequenceManagerUI(SequenceManager * _manager) :
	BaseManagerShapeShifterUI<SequenceManager, Sequence, SequenceUI>("Sequences", _manager)
{
	addItemText = "Add Sequence";
	addExistingItems();
}

SequenceManagerUI::~SequenceManagerUI()
{
}
