/*
  ==============================================================================

    CVPanel.h
    Created: 15 Feb 2018 3:34:56pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CVGroupManager.h"

class CVPanel :
	public ShapeShifterContentComponent
{
public:
	CVPanel(const String & name);
	~CVPanel();

	BaseManagerUI<CVGroupManager, CVGroup, BaseItemUI<CVGroup>> groupManagerUI;

	static CVPanel * create(const String &name) { return new CVPanel(name); }
};