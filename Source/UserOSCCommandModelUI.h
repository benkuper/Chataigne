/*
  ==============================================================================

    UserOSCCommandModelUI.h
    Created: 15 Jan 2017 5:17:01pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELUI_H_INCLUDED
#define USEROSCCOMMANDMODELUI_H_INCLUDED

#include "BaseItemUI.h"
#include "UserOSCCommandModel.h"

class UserOSCCommandModelUI :
	public BaseItemUI<UserOSCCommandModel>
{
public:
	UserOSCCommandModelUI(UserOSCCommandModel * item);
	virtual ~UserOSCCommandModelUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelUI)
};



#endif  // USEROSCCOMMANDMODELUI_H_INCLUDED
