/*
  ==============================================================================

    AutomationEditor.h
    Created: 5 Feb 2017 2:58:27pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONEDITOR_H_INCLUDED
#define AUTOMATIONEDITOR_H_INCLUDED

#include "AutomationUI.h"

class AutomationEditor :
	public GenericControllableContainerEditor
{
public:
	AutomationEditor(Automation * automation, bool isRoot);
	~AutomationEditor();

	Automation * automation;
	ScopedPointer<AutomationUI> automationUI;

	void resizedInternalContent(Rectangle<int> &r) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationEditor)
};



#endif  // AUTOMATIONEDITOR_H_INCLUDED
