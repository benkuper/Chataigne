/*
  ==============================================================================

    SequenceAudioCommand.h
    Created: 27 Sep 2020 10:57:00am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"

class SequenceModule;

class SequenceAudioCommand :
	public BaseCommand,
	public EngineListener
{
public:
	SequenceAudioCommand(SequenceModule* _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	virtual ~SequenceAudioCommand();

	enum ActionType { SET_VOLUME };

	ActionType actionType;
	SequenceModule* sequenceModule;

	TargetParameter* target;
	FloatParameter * time;
	FloatParameter * value;
	BoolParameter* stopAtFinish;
	Automation* automation;

	var dataToLoad;

	virtual void triggerInternal(int iterationIndex) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
};
