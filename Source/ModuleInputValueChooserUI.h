/*
  ==============================================================================

    ModuleInputValueChooserUI.h
    Created: 8 Feb 2017 9:38:57am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEINPUTVALUECHOOSERUI_H_INCLUDED
#define MODULEINPUTVALUECHOOSERUI_H_INCLUDED

#include "ModuleManagerUI.h"


class ModuleInputValueChooserUI :
	public TargetParameterUI
{
public:
	ModuleInputValueChooserUI(TargetParameter * p, bool parametersOnly);
	virtual ~ModuleInputValueChooserUI();

	bool parametersOnly;

	void updateLabel() override;
	void showPopupAndGetTarget() override;//can be overriden to get specific PopupMenu


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleInputValueChooserUI)
};


#endif  // MODULEINPUTVALUECHOOSERUI_H_INCLUDED
