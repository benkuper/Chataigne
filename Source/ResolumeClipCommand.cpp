/*
  ==============================================================================

    ResolumeClipCommand.cpp
    Created: 3 Nov 2016 12:34:21pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeClipCommand.h"

ResolumeClipCommand::ResolumeClipCommand(ResolumeModule * _module, CommandContext context, var params) :
	ResolumeBaseCommand(_module,context,params)
{
	connectParam = argumentsContainer.addIntParameter("Connect", "", 1, 1, 1);
	argumentsContainer.hideInEditor = true;

	address->isEditable = false;

	rebuildAddress();
}

ResolumeClipCommand::~ResolumeClipCommand()
{

}
