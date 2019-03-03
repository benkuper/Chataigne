/*
  ==============================================================================

    OSCOutputEditor.h
    Created: 3 Mar 2019 12:15:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../OSCModule.h"

class OSCOutputEditor :
	public BaseItemEditor
{
public:
	OSCOutputEditor(OSCOutput * output, bool isRoot = false);
	~OSCOutputEditor();

	TextButton zeroconfMenu;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;

	void showMenuAndSetupOutput();

	// Inherited via Listener
	virtual void buttonClicked(Button *) override;
};