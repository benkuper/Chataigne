/*
  ==============================================================================

    ConsequenceUI.h
    Created: 28 Oct 2016 8:08:09pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONSEQUENCEUI_H_INCLUDED
#define CONSEQUENCEUI_H_INCLUDED


#include "BaseItemUI.h"
#include "Consequence.h"

class ConsequenceUI :
	public BaseItemUI<Consequence>
{
public:
	ConsequenceUI(Consequence *);
	virtual ~ConsequenceUI();

	void mouseDown(const MouseEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceUI)
};





#endif  // CONSEQUENCEUI_H_INCLUDED
