/*
  ==============================================================================

    ModuleCommandTesterEditor.h
    Created: 11 Mar 2019 2:20:26pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/ui/BaseCommandHandlerEditor.h"

class ModuleCommandTesterEditor :
	public BaseCommandHandlerEditor
{
public:
	ModuleCommandTesterEditor(ModuleCommandTester * tester, bool isRoot = false);
	~ModuleCommandTesterEditor();

	std::unique_ptr<BoolToggleUI> autoTriggerUI;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
};