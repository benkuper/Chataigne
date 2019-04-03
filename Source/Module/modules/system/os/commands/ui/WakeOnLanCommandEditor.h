/*
  ==============================================================================

    WakeOnLanCommandEditor.h
    Created: 2 Apr 2019 5:56:12pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../WakeOnLanCommand.h"

class WakeOnLanCommandEditor :
	public GenericControllableContainerEditor
{
public:
	WakeOnLanCommandEditor(WakeOnLanCommand * wolCommand, bool isRoot);
	~WakeOnLanCommandEditor();

	TextButton zeroconfMenu;
	WakeOnLanCommand * wolCommand;

	void resizedInternalHeader(Rectangle<int> &r) override;

	void showMenuAndSetupMacAdddress();
	void buttonClicked(Button * b) override;
};