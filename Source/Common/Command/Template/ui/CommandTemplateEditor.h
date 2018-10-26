/*
  ==============================================================================

    CommandTemplateEditor.h
    Created: 31 May 2018 11:29:07am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CommandTemplate.h"

class CommandTemplateEditor :
	public GenericControllableContainerEditor
{
public:
	CommandTemplateEditor(CommandTemplate * t, bool isRoot);
	~CommandTemplateEditor();

	ScopedPointer<TriggerButtonUI> triggerUI;

	void resizedInternalHeader(Rectangle<int> &r) override;
};