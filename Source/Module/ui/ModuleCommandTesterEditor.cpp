/*
  ==============================================================================

	ModuleCommandTesterEditor.cpp
	Created: 11 Mar 2019 2:20:26pm
	Author:  bkupe

  ==============================================================================
*/

#include "ModuleCommandTesterEditor.h"

ModuleCommandTesterEditor::ModuleCommandTesterEditor(ModuleCommandTester * tester, bool isRoot) :
	BaseCommandHandlerEditor(tester, isRoot)
{
	autoTriggerUI = tester->autoTrigger->createToggle();
	addAndMakeVisible(autoTriggerUI);
	
}

ModuleCommandTesterEditor::~ModuleCommandTesterEditor()
{
}

void ModuleCommandTesterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	BaseCommandHandlerEditor::resizedInternalHeaderItemInternal(r);
	autoTriggerUI->setBounds(r.removeFromRight(80).reduced(2));
}
