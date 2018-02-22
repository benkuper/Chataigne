/*
  ==============================================================================

    CVGroupManagerUI.h
    Created: 22 Feb 2018 3:42:48pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CVGroupManager.h"

class CVGroupManagerUI :
	public BaseManagerShapeShifterUI<BaseManager<CVGroup>, CVGroup, BaseItemUI<CVGroup>>
{
public:
	CVGroupManagerUI(const String &contentName);
	~CVGroupManagerUI();

	static CVGroupManagerUI * create(const String &name) { return new CVGroupManagerUI(name); }
};