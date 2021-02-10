/*
  ==============================================================================

    BaseCommandEditor.h
    Created: 21 Dec 2020 11:09:27pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../BaseCommand.h"
#include "../../ParameterLink/ui/LinkableParameterEditor.h"

class BaseCommandContainerEditor :
	public ParamLinkContainerEditor,
	public BaseCommand::CommandListener
{
public:
	BaseCommandContainerEditor(BaseCommand* command, ControllableContainer * container, bool isRoot);
	~BaseCommandContainerEditor();

	BaseCommand* baseCommand;
	virtual InspectableEditor* getEditorUIForContainer(ControllableContainer* cc) override;
};
