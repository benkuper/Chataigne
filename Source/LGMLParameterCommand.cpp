/*
  ==============================================================================

    LGMLParameterCommand.cpp
    Created: 17 May 2017 7:41:47pm
    Author:  Ben

  ==============================================================================
*/

#include "LGMLParameterCommand.h"
#include "LGMLModule.h"

LGMLParameterCommand::LGMLParameterCommand(LGMLModule * _module, CommandContext context, var params) :
	OSCCommand(_module,context,params),
	lgmlModule(_module)
{
}

LGMLParameterCommand::~LGMLParameterCommand()
{
}

void LGMLParameterCommand::onContainerParameterChanged(Parameter *)
{

}