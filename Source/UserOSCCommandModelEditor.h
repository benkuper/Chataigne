/*
  ==============================================================================

    UserOSCCommandModelEditor.h
    Created: 15 Jan 2017 5:01:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELEDITOR_H_INCLUDED
#define USEROSCCOMMANDMODELEDITOR_H_INCLUDED

#include "GenericControllableContainerEditor.h"
#include "UserOSCCommandModel.h"

class UserOSCCommandModelEditor :
	public GenericControllableContainerEditor
{
public:
	UserOSCCommandModelEditor(UserOSCCommandModel * model, bool isRoot);
	~UserOSCCommandModelEditor();

	UserOSCCommandModel * model;

	void mouseDown(const MouseEvent &e) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelEditor)
};



#endif  // USEROSCCOMMANDMODELEDITOR_H_INCLUDED
