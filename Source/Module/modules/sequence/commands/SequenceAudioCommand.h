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
	SequenceAudioCommand(SequenceModule* _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
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

	virtual void triggerInternal(int multiplexIndex) override;

	template<class T>
	T* getTargetAs(int multiplexIndex)
	{
		if (ParameterLink* pl = getLinkedParam(target))
		{
			return dynamic_cast<T*>(pl->getLinkedTargetContainer());
		}

		return nullptr;
	}

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
