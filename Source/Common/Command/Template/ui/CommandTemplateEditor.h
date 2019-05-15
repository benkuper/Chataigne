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
	public BaseItemEditor
{
public:
	CommandTemplateEditor(CommandTemplate * t, bool isRoot);
	~CommandTemplateEditor();

	std::unique_ptr<TriggerButtonUI> triggerUI;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
};