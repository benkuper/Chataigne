/*
  ==============================================================================

    UserOSCCommand.cpp
    Created: 15 Jan 2017 4:43:14pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommand.h"
#include "UserOSCCommandEditor.h"

UserOSCCommand::UserOSCCommand(CustomOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	module(_module)
{
	//get model from module depending on params
}

UserOSCCommand::~UserOSCCommand()
{
}

InspectableEditor * UserOSCCommand::getEditor(bool isRoot)
{
	return new UserOSCCommandEditor(this, isRoot);
}
