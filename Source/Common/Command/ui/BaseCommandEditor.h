/*
  ==============================================================================

    BaseCommandEditor.h
    Created: 21 Dec 2020 11:09:27pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../BaseCommand.h"

class BaseCommandEditor :
	public GenericControllableContainerEditor,
	public BaseCommand::CommandListener
{
public:
	BaseCommandEditor(BaseCommand* command, bool isRoot);
	~BaseCommandEditor();

	BaseCommand* baseCommand;

	virtual InspectableEditor* getEditorUIForControllable(Controllable* c) override;

};