/*
  ==============================================================================

    ConsequenceManagerUI.h
    Created: 28 Oct 2016 8:08:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONSEQUENCEMANAGERUI_H_INCLUDED
#define CONSEQUENCEMANAGERUI_H_INCLUDED



#include "ConsequenceUI.h"
#include "BaseManagerUI.h"
#include "ConsequenceManager.h"

class ConsequenceManagerUI :
	public BaseManagerUI<ConsequenceManager, Consequence, ConsequenceUI>
{
public:

	ConsequenceManagerUI(ConsequenceManager *_manager);
	~ConsequenceManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceManagerUI)
};



#endif  // CONSEQUENCEMANAGERUI_H_INCLUDED
