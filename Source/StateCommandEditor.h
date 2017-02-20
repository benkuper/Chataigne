/*
  ==============================================================================

    StateCommandEditor.h
    Created: 20 Feb 2017 3:19:35pm
    Author:  Ben

  ==============================================================================
*/

#ifndef STATECOMMANDEDITOR_H_INCLUDED
#define STATECOMMANDEDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "StateCommand.h"
#include "StateChooserUI.h"

class StateCommandEditor :
	public InspectableEditor
{
public:
	StateCommandEditor(StateCommand * stateCommand, bool isRoot);
	~StateCommandEditor();

	StateCommand * stateCommand;
	ScopedPointer<StateChooserUI> chooserUI;

	void resized() override;
};


#endif  // STATECOMMANDEDITOR_H_INCLUDED
