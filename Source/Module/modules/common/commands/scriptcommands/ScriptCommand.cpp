/*
  ==============================================================================

    ScriptCommand.cpp
    Created: 1 Aug 2018 5:47:34pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptCommand.h"

ScriptCommand::ScriptCommand(Module * module, CommandContext context, var data) :
	BaseCommand(module, context, data)
{
	//load params here

}

ScriptCommand::~ScriptCommand()
{
}
