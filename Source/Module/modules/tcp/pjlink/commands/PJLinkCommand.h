/*
  ==============================================================================

	PJLinkCommand.h
	Created: 1 Nov 2022 12:23:25pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class PJLinkCommand :
	public BaseCommand
{
public:
	PJLinkCommand(PJLinkModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~PJLinkCommand();

	enum ActionType { POWER, SHUTTER, CUSTOM };

	ActionType action;

	String prefix;

	PJLinkModule* pjlinkModule;

	IntParameter* projectorID;
	EnumParameter* op;
	StringParameter* message;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new PJLinkCommand((PJLinkModule*)module, context, params, multiplex); }
};
