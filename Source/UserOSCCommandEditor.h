/*
  ==============================================================================

    UserOSCCommandEditor.h
    Created: 15 Jan 2017 5:03:15pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDEDITOR_H_INCLUDED
#define USEROSCCOMMANDEDITOR_H_INCLUDED

#include "UserOSCCommand.h"

class UserOSCCommandEditor :
	public GenericControllableContainerEditor
{
public:
	UserOSCCommandEditor(UserOSCCommand * command, bool isRoot);
	~UserOSCCommandEditor();

	UserOSCCommand * command;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandEditor)
};



#endif  // USEROSCCOMMANDEDITOR_H_INCLUDED
