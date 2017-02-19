/*
  ==============================================================================

    ResolumeBaseCommandEditor.cpp
    Created: 19 Feb 2017 1:39:03pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeBaseCommandEditor.h"

ResolumeBaseCommandEditor::ResolumeBaseCommandEditor(ResolumeBaseCommand * _command, bool isRoot) :
	GenericControllableContainerEditor(_command,isRoot),
	resolumeCommand(_command)
{

}

ResolumeBaseCommandEditor::~ResolumeBaseCommandEditor()
{

}

void ResolumeBaseCommandEditor::controllableFeedbackUpdate(Controllable * c)
{
	if (c == resolumeCommand->levelParam)
	{
		resetAndBuild();
	}
}
