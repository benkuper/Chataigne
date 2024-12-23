/*
  ==============================================================================

    ScriptCallbackCommand.h
    Created: 12 Jul 2019 10:43:37am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ScriptCallbackCommand :
	public BaseCommand
{
public:
	ScriptCallbackCommand(Module * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~ScriptCallbackCommand();

	EnumParameter* moduleMethods;

	void triggerInternal(int multiplexIndex) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;


	static ScriptCallbackCommand* create(ControllableContainer* c, CommandContext context, var params, Multiplex * multiplex = nullptr) { return new ScriptCallbackCommand((Module *)c, context, params, multiplex); }
};