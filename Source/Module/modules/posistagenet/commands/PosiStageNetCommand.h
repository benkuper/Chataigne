/*
  ==============================================================================

    PosiStageNetCommand.h
    Created: 17 May 2022 12:04:47am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PosiStageNetCommand :
	public BaseCommand
{
public:
	PosiStageNetCommand(PosiStageNetModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~PosiStageNetCommand();

	PosiStageNetModule* psnModule;

	enum Action { SET_POSITION };
	Action action;

	IntParameter* slot;
	Point3DParameter* position;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new PosiStageNetCommand((PosiStageNetModule*)module, context, params, multiplex); }
};
