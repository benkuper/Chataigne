/*
  ==============================================================================

    ModuleCommand.cpp
    Created: 8 Dec 2016 2:36:21pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleCommand.h"
#include "Module.h"

ModuleCommand::ModuleCommand(Module * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	module(_module)
{

}

ModuleCommand::~ModuleCommand()
{
}
