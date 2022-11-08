/*
  ==============================================================================

    OSCQueryModuleEditor.h
    Created: 3 Mar 2019 12:36:47pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class GenericOSCQueryValueContainerEditor :
	public GenericControllableContainerEditor
{
public:
	GenericOSCQueryValueContainerEditor(GenericOSCQueryValueContainer* cc, bool isRoot);
	~GenericOSCQueryValueContainerEditor();

	std::unique_ptr<BoolToggleUI> enableListenUI;
	std::unique_ptr<BoolToggleUI> syncUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
};

class OSCQueryModuleOutputEditor :
	public EnablingControllableContainerEditor
{
public:
	OSCQueryModuleOutputEditor(OSCQueryOutput * output, bool isRoot = false);
	virtual ~OSCQueryModuleOutputEditor();

	TextButton zeroconfMenu;

	void resizedInternalHeader(Rectangle<int> &r) override;

	void showMenuAndSetupOutput();

	// Inherited via Listener
	virtual void buttonClicked(Button *) override;
};