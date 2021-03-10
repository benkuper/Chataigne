/*
  ==============================================================================

    CommunityModuleInfoEditor.h
    Created: 29 Jan 2019 3:53:01pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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
	std::unique_ptr<TriggerButtonUI> installBT;
	std::unique_ptr<TriggerButtonUI> uninstallBT;


	void setCollapsed(bool  value, bool force = false, bool animate = true, bool doNotRebuild = false) override;

	void resetAndBuild() override {} //do nothing

	void updateVersionUI();

	void controllableFeedbackUpdate(Controllable * c) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void mouseDown(const MouseEvent &e) override;
};