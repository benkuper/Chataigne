/*
  ==============================================================================

    SequenceCommandEditor.h
    Created: 20 Feb 2017 4:15:55pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SEQUENCECOMMANDEDITOR_H_INCLUDED
#define SEQUENCECOMMANDEDITOR_H_INCLUDED



#include "SequenceCommand.h"
#include "SequenceChooserUI.h"

class SequenceCommandEditor :
	public InspectableEditor
{
public:
	SequenceCommandEditor(SequenceCommand * sequenceCommand, bool isRoot);
	virtual ~SequenceCommandEditor();

	SequenceCommand * sequenceCommand;
	ScopedPointer<SequenceChooserUI> chooserUI;

	virtual void resized() override;
};



#endif  // SEQUENCECOMMANDEDITOR_H_INCLUDED
