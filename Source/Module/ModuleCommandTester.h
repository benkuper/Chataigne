/*
  ==============================================================================

	ModuleCommandTester.h
	Created: 11 Mar 2019 2:13:19pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ModuleCommandTester :
	public BaseCommandHandler
{
public:
	ModuleCommandTester(Module* module);
	~ModuleCommandTester();

	BoolParameter* autoTrigger;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};