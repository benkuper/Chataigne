/*
  ==============================================================================

    SequenceManagerUI.h
    Created: 28 Oct 2016 8:13:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEMANAGERUI_H_INCLUDED
#define SEQUENCEMANAGERUI_H_INCLUDED


#include "SequenceManager.h"

#include "SequenceUI.h"

class SequenceManagerUI :
	public BaseManagerShapeShifterUI<SequenceManager, Sequence, SequenceUI>
{
public:
	SequenceManagerUI(const String &contentName, SequenceManager *_manager);
	~SequenceManagerUI();

	static SequenceManagerUI * create(const String &contentName) { return new SequenceManagerUI(contentName, SequenceManager::getInstance()); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceManagerUI)
};





#endif  // SEQUENCEMANAGERUI_H_INCLUDED
