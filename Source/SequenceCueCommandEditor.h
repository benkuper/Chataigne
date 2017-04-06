/*
  ==============================================================================

    SequenceCueCommandEditor.h
    Created: 6 Apr 2017 12:29:26pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SEQUENCECUECOMMANDEDITOR_H_INCLUDED
#define SEQUENCECUECOMMANDEDITOR_H_INCLUDED

#include "SequenceCommandEditor.h"
#include "SequenceCueCommand.h"

class SequenceCueCommandEditor :
	public SequenceCommandEditor,
	public ControllableContainer::ContainerAsyncListener
{
public:
	SequenceCueCommandEditor(SequenceCueCommand * command, bool isRoot);
	~SequenceCueCommandEditor();

	SequenceCueCommand * cueCommand;

	ScopedPointer<SequenceChooserUI> cueTargetUI;

	void resized() override;

	void newMessage(const ContainerAsyncEvent &e) override;
};


#endif  // SEQUENCECUECOMMANDEDITOR_H_INCLUDED
