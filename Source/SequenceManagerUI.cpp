/*
  ==============================================================================

    SequenceManagerUI.cpp
    Created: 28 Oct 2016 8:13:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceManagerUI.h"

SequenceManagerUI::SequenceManagerUI(const String &contentName, SequenceManager * _manager) :
	BaseManagerShapeShifterUI<SequenceManager, Sequence, SequenceUI>(contentName, _manager)
{
	addItemText = "Add Sequence";
	addExistingItems();
}

SequenceManagerUI::~SequenceManagerUI()
{
}
