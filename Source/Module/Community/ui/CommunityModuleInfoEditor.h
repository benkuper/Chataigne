/*
  ==============================================================================

    CommunityModuleInfoEditor.h
    Created: 29 Jan 2019 3:53:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../CommunityModuleInfo.h"

class CommunityModuleInfoEditor :
	public InspectableEditor
{
public:
	CommunityModuleInfoEditor(CommunityModuleInfo * cmi, bool isRoot);
	~CommunityModuleInfoEditor();

	CommunityModuleInfo * cmi;
	Label urlLabel;
	TextEditor description;
	ScopedPointer<TriggerButtonUI> installBT;
	ScopedPointer<TriggerButtonUI> uninstallBT;


	void controllableFeedbackUpdate(Controllable * c) override;
	void resizedInternalContent(Rectangle<int> &r) override;
};