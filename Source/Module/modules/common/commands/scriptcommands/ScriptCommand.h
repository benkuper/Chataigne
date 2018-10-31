/*
  ==============================================================================

    ScriptCommand.h
    Created: 1 Aug 2018 5:47:34pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h" 

class ScriptCommand :
	public BaseCommand
{
public:
	ScriptCommand(Module * module, CommandContext context, var data);
	~ScriptCommand();

	OwnedArray<ControllableContainer> paramContainers;
	Identifier callback;
	Array<Parameter *> scriptParams; 

	void createControllablesForContainer(var data, ControllableContainer * cc);
	Controllable * getControllableForJSONDefinition(const String &name, var def);

	void trigger() override;


	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params)  { return new ScriptCommand((Module *)module, context, params); }
};