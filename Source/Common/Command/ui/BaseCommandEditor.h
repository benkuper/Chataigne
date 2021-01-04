/*
  ==============================================================================

    BaseCommandEditor.h
    Created: 21 Dec 2020 11:09:27pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../BaseCommand.h"

class BaseCommandContainerEditor :
	public GenericControllableContainerEditor,
	public BaseCommand::CommandListener
{
public:
	BaseCommandContainerEditor(BaseCommand* command, ControllableContainer * container, bool isRoot);
	~BaseCommandContainerEditor();

	BaseCommand* baseCommand;
	virtual InspectableEditor* getEditorUIForControllable(Controllable * c) override;
	virtual InspectableEditor* getEditorUIForContainer(ControllableContainer* cc) override;
};
