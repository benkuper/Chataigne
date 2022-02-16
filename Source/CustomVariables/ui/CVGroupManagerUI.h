/*
  ==============================================================================

    CVGroupManagerUI.h
    Created: 22 Feb 2018 3:42:48pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CVGroupManagerUI :
	public BaseManagerShapeShifterUI<BaseManager<CVGroup>, CVGroup, CVGroupUI>
{
public:
	CVGroupManagerUI(const String &contentName);
	~CVGroupManagerUI();

	static CVGroupManagerUI * create(const String &name) { return new CVGroupManagerUI(name); }
};