/*
  ==============================================================================

    UserOSCCommandModel.h
    Created: 15 Jan 2017 4:53:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODEL_H_INCLUDED
#define USEROSCCOMMANDMODEL_H_INCLUDED

#include "BaseItem.h"

class UserOSCCommandModel :
	public BaseItem
{
public:
	UserOSCCommandModel();
	~UserOSCCommandModel();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModel)
};



#endif  // USEROSCCOMMANDMODEL_H_INCLUDED
