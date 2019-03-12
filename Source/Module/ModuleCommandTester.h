/*
  ==============================================================================

    ModuleCommandTester.h
    Created: 11 Mar 2019 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommandHandler.h"

class ModuleCommandTester :
	public BaseCommandHandler
{
public:
	ModuleCommandTester(Module * module);
	~ModuleCommandTester();

	BoolParameter * autoTrigger;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	InspectableEditor * getEditor(bool isRoot) override;
};