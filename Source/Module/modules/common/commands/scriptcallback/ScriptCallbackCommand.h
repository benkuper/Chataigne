/*
  ==============================================================================

    ScriptCallbackCommand.h
    Created: 12 Jul 2019 10:43:37am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "Module/Module.h"

class ScriptCallbackCommand :
	public BaseCommand
{
public:
	ScriptCallbackCommand(Module * module, CommandContext context, var params);
	~ScriptCallbackCommand();

	EnumParameter* moduleMethods;

	void triggerInternal() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	static ScriptCallbackCommand* create(ControllableContainer* c, CommandContext context, var params) { return new ScriptCallbackCommand((Module *)c, context, params); }
};