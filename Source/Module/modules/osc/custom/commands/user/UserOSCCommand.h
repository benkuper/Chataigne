/*
  ==============================================================================

    UserOSCCommand.h
    Created: 15 Jan 2017 4:43:14pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMAND_H_INCLUDED
#define USEROSCCOMMAND_H_INCLUDED

#include "../../../commands/OSCCommand.h"
#include "../../CustomOSCModule.h"
#include "model/UserOSCCommandModel.h"

class UserOSCCommand :
	public OSCCommand,
	public UserOSCCommandModel::ModelListener
{
public:
	UserOSCCommand(CustomOSCModule * module, CommandContext context, var params);
	virtual ~UserOSCCommand();

	CustomOSCModule * cModule;
	UserOSCCommandModel * model;
	WeakReference<Inspectable> modelRef;

	void rebuildArgsFromModel();

	void commandModelAddressChanged(UserOSCCommandModel *) override;
	void commandModelArgumentsChanged(UserOSCCommandModel *) override;

	static UserOSCCommand * create(ControllableContainer * module, CommandContext context, var params) { return new UserOSCCommand((CustomOSCModule *)module, context, params); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommand)
};

#endif  // USEROSCCOMMAND_H_INCLUDED
