/*
  ==============================================================================

    LGMLParameterCommand.h
    Created: 17 May 2017 7:41:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "OSCCommand.h"
class LGMLModule;

class LGMLParameterCommand :
	public OSCCommand
{
public:
	LGMLParameterCommand(LGMLModule * _module, CommandContext context, var params);
	virtual ~LGMLParameterCommand();

	LGMLModule * lgmlModule;

	virtual void onContainerParameterChanged(Parameter *) override;

	//InspectableEditor * getEditor(bool isRoot) override;
};

