/*
  ==============================================================================

    ModuleCommandTester.cpp
    Created: 11 Mar 2019 2:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "ModuleCommandTester.h"
#include "ui/ModuleCommandTesterEditor.h"

ModuleCommandTester::ModuleCommandTester(Module * module) :
	BaseCommandHandler("Command Tester", ACTION, module)
{

	userCanRemove = false;
	canBeDisabled = false;
	canBeReorderedInEditor = false;
	nameCanBeChangedByUser = false;

	autoTrigger = addBoolParameter("Auto Trigger", "If checked, every change of parameter will trigger the command", false);
	autoTrigger->hideInEditor = true;
}

ModuleCommandTester::~ModuleCommandTester()
{
}

void ModuleCommandTester::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	BaseCommandHandler::onControllableFeedbackUpdateInternal(cc, c);

	if (cc == command.get() && autoTrigger->boolValue())
	{
		trigger->trigger();
	}
}

InspectableEditor * ModuleCommandTester::getEditor(bool isRoot)
{
	return new ModuleCommandTesterEditor(this, isRoot);
}
