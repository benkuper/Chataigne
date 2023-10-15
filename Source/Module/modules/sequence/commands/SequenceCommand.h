/*
  ==============================================================================

    SequenceCommand.h
    Created: 20 Feb 2017 2:12:09pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class SequenceModule;

class SequenceCommand :
	public BaseCommand,
	public EngineListener
{
public:
	SequenceCommand(SequenceModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	virtual ~SequenceCommand();

	enum ActionType { PLAY_SEQUENCE, PLAY_SEQUENCE_AT, PLAY_MULTI_SEQUENCES, PAUSE_SEQUENCE, STOP_SEQUENCE, STOP_ALL_SEQUENCES, TOGGLE_SEQUENCE, ENABLE_LAYER, DISABLE_LAYER, TOGGLE_LAYER, SET_TIME, MOVE_TIME, GOTO_CUE, GOTO_PREV_CUE, GOTO_NEXT_CUE, SET_TRIGGER_ENABLED, SET_EDITING_SEQUENCE, SET_EDITING_SEQUENCE_AT };

	ActionType actionType;
	SequenceModule * sequenceModule;

	BoolParameter * playFromStart;

	IntParameter* minIndex;
	IntParameter* maxIndex;
	BoolParameter* loopMulti;
	IntParameter* currentSequenceIndex;
	Trigger* resetIncrement;

	TargetParameter * target;
	Parameter * value;

	var dataToLoad;


	virtual void triggerInternal(int multiplexIndex) override;

	virtual void onContainerTriggerTriggered(Trigger* t) override;
	virtual void onContainerParameterChanged(Parameter* p) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
