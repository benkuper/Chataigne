/*
  ==============================================================================

    CustomOSCCommandEditor.cpp
    Created: 20 Dec 2016 8:03:33pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandEditor.h"

CustomOSCCommandEditor::CustomOSCCommandEditor(CustomOSCCommand * _command, bool isRoot) :
	GenericControllableContainerEditor(_command, isRoot),
	command(_command)
{

}

CustomOSCCommandEditor::~CustomOSCCommandEditor()
{
}