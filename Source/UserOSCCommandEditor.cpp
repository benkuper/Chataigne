/*
  ==============================================================================

    UserOSCCommandEditor.cpp
    Created: 15 Jan 2017 5:03:15pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandEditor.h"

UserOSCCommandEditor::UserOSCCommandEditor(UserOSCCommand * _command, bool isRoot) :
	GenericControllableContainerEditor(_command, isRoot),
	command(_command)
{
}

UserOSCCommandEditor::~UserOSCCommandEditor()
{
}
