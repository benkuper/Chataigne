/*
  ==============================================================================

    ModuleUI.h
    Created: 8 Dec 2016 2:36:51pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEUI_H_INCLUDED
#define MODULEUI_H_INCLUDED

#include "Module.h"

class ModuleUI :
	public BaseItemUI<Module>,
	public Module::ModuleListener
{
public:
	ModuleUI(Module * module);
	virtual ~ModuleUI();

	void resizedInternalHeader(Rectangle<int> &r) override;

	ScopedPointer<TriggerImageUI> inActivityUI;
	ScopedPointer<TriggerImageUI> outActivityUI;

	void moduleIOConfigurationChanged() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleUI)
};






#endif  // MODULEUI_H_INCLUDED
