/*
  ==============================================================================

    ModuleChooserUI.h
    Created: 13 Mar 2017 4:22:50pm
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef MODULECHOOSERUI_H_INCLUDED
#define MODULECHOOSERUI_H_INCLUDED


#include "ModuleManagerUI.h"
#include "TargetParameterUI.h"

class ModuleChooserUI
{
public:
	ModuleChooserUI();
	virtual ~ModuleChooserUI();


	Module * showPopupAndGetTarget(const String &typeFilter);//can be overriden to get specific PopupMenu

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleChooserUI)
};



#endif  // MODULECHOOSERUI_H_INCLUDED
