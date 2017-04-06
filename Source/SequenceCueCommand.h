/*
  ==============================================================================

    SequenceCueCommand.h
    Created: 6 Apr 2017 12:29:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SEQUENCECUECOMMAND_H_INCLUDED
#define SEQUENCECUECOMMAND_H_INCLUDED

#include "SequenceCommand.h"

class SequenceCueCommand :
	public SequenceCommand
{
public:
	SequenceCueCommand(SequenceModule * _module, CommandContext context, var params);
	virtual ~SequenceCueCommand();

	TargetParameter * targetCue;

	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);
	InspectableEditor * getEditor(bool isRoot) override;
};



#endif  // SEQUENCECUECOMMAND_H_INCLUDED
