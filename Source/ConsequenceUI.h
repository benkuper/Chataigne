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
#include "CommandChooserUI.h"

class ConsequenceUI :
	public BaseItemUI<Consequence>,
	public CommandChooserUI::Listener,
	public Consequence::ConsequenceListener
{
public:
	ConsequenceUI(Consequence *);
	virtual ~ConsequenceUI();

	CommandChooserUI chooser;
	ScopedPointer<InspectableEditor> commandEditor;

	void resized() override;

	void updateChooserLabel();

	void updateCommandUI();
	void definitionChosen(CommandDefinition * d) override;

	void consequenceCommandChanged(Consequence*) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceUI)
};





#endif  // CONSEQUENCEUI_H_INCLUDED
