/*
  ==============================================================================

    ResolumeBaseCommandEditor.h
    Created: 19 Feb 2017 1:39:03pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../ResolumeBaseCommand.h"
#include "Common/Command/ui/BaseCommandEditor.h"

class ResolumeBaseCommandEditor :
	public BaseCommandEditor
{
public:
	ResolumeBaseCommandEditor(ResolumeBaseCommand * _command, bool isRoot);
	virtual ~ResolumeBaseCommandEditor();

	ResolumeBaseCommand * resolumeCommand;

	void controllableFeedbackUpdate(Controllable *c) override;

};