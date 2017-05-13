/*
  ==============================================================================

    CustomOSCCommandEditor.h
    Created: 20 Dec 2016 8:03:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDEDITOR_H_INCLUDED
#define CUSTOMOSCCOMMANDEDITOR_H_INCLUDED

#include "CustomOSCCommand.h"

class CustomOSCCommandEditor :
	public GenericControllableContainerEditor
{
public:
	CustomOSCCommandEditor(CustomOSCCommand * command, bool isRoot);
	~CustomOSCCommandEditor();

	WeakReference<CustomOSCCommand> command;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommandEditor)
};



#endif  // CUSTOMOSCCOMMANDEDITOR_H_INCLUDED
