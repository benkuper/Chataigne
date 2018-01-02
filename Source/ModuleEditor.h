/*
  ==============================================================================

    ModuleEditor.h
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEEDITOR_H_INCLUDED
#define MODULEEDITOR_H_INCLUDED

#include "Module.h"

#include "BaseCommandHandlerEditor.h"

class ModuleEditor :
	public BaseItemEditor,
	public Module::ModuleListener
{
public:
	ModuleEditor(Module * module, bool isRoot);
	virtual ~ModuleEditor();

	Module * module;
	ScopedPointer<BoolToggleUI> logIncomingUI;
	ScopedPointer<BoolToggleUI> logOutgoingUI;

	virtual void resizedInternalHeaderItemInternal(Rectangle<int> &r) override; 
	
	virtual void moduleIOConfigurationChanged() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleEditor)
};



#endif  // MODULEEDITOR_H_INCLUDED
