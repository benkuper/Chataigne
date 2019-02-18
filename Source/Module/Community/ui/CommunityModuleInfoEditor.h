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
	public GenericControllableContainerEditor
{
public:
	CommunityModuleInfoEditor(CommunityModuleInfo * cmi, bool isRoot);
	~CommunityModuleInfoEditor();

	CommunityModuleInfo * cmi;
	Label urlLabel;
	Label versionsLabel;
	TextEditor description;
	ScopedPointer<TriggerButtonUI> installBT;
	ScopedPointer<TriggerButtonUI> uninstallBT;


	void resetAndBuild() override {} //do nothing

	void updateVersionUI();

	void controllableFeedbackUpdate(Controllable * c) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void mouseDown(const MouseEvent &e) override;
};