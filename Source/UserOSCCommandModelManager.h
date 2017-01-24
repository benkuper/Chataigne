/*
  ==============================================================================

    UserOSCCommandModelManager.h
    Created: 15 Jan 2017 4:52:31pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELMANAGER_H_INCLUDED
#define USEROSCCOMMANDMODELMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "UserOSCCommandModel.h"

class UserOSCCommandModelManager :
	public BaseManager<UserOSCCommandModel>
{
public:
	UserOSCCommandModelManager();
	virtual ~UserOSCCommandModelManager();
	

	void addItemInternal(UserOSCCommandModel *, var data) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManager)
};



#endif  // USEROSCCOMMANDMODELMANAGER_H_INCLUDED
