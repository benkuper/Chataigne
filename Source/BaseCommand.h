/*
  ==============================================================================

    BaseCommand.h
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASECOMMAND_H_INCLUDED
#define BASECOMMAND_H_INCLUDED

#include "JuceHeader.h"

class Module;

enum CommandContext { ACTION, MAPPING, BOTH };

class BaseCommand :
	public ControllableContainer
{
public:
	BaseCommand(Module * module, CommandContext context, var params);
	virtual ~BaseCommand() {}

	CommandContext context;
	Module * module;
	var params;

	Array<Parameter *> targetMappingParameters;
	void setTargetMappingParameterAt(Parameter * p,int index);
	void clearTargetMappingParameters();

	virtual void trigger() {} //for trigger context, to override
	virtual void setValue(var value); //for mapping context

	virtual void loadPreviousCommandData(var data)
	{
		//default behavior is loading doing  nothing, can override that to trying hot swap of commands
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommand)
};

#endif  // BASECOMMAND_H_INCLUDED
