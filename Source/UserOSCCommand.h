/*
  ==============================================================================

    UserOSCCommand.h
    Created: 15 Jan 2017 4:43:14pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMAND_H_INCLUDED
#define USEROSCCOMMAND_H_INCLUDED

#include "OSCCommand.h"
#include "CustomOSCModule.h"
#include "UserOSCCommandModel.h"

class UserOSCCommand :
	public OSCCommand
{
public:
	UserOSCCommand(CustomOSCModule * module, CommandContext context, var params);
	virtual ~UserOSCCommand();

	CustomOSCModule * cModule;
	UserOSCCommandModel * model;

	void rebuildArgsFromModel();

	InspectableEditor * getEditor(bool /*isRoot*/) override;
	static UserOSCCommand * create(ControllableContainer * module, CommandContext context, var params) { return new UserOSCCommand((CustomOSCModule *)module, context, params); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommand)
};

#endif  // USEROSCCOMMAND_H_INCLUDED
