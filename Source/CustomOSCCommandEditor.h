/*
  ==============================================================================

    CustomOSCCommandEditor.h
    Created: 20 Dec 2016 8:03:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDEDITOR_H_INCLUDED
#define CUSTOMOSCCOMMANDEDITOR_H_INCLUDED

#include "GenericControllableContainerEditor.h"
#include "CustomOSCCommand.h"

class CustomOSCCommandEditor :
	public GenericControllableContainerEditor
{
public:
	CustomOSCCommandEditor(CustomOSCCommand * command, bool isRoot);
	~CustomOSCCommandEditor();

	CustomOSCCommand * command;

	void mouseDown(const MouseEvent &e) override;
};



#endif  // CUSTOMOSCCOMMANDEDITOR_H_INCLUDED
