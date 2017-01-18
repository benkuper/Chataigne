/*
  ==============================================================================

    ModuleEditor.h
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEEDITOR_H_INCLUDED
#define MODULEEDITOR_H_INCLUDED

#include "BaseItemEditor.h"
#include "Module.h"
#include "BoolToggleUI.h"

class ModuleEditor :
	public BaseItemEditor
{
public:
	ModuleEditor(Module * module, bool isRoot);
	virtual ~ModuleEditor();

	Module * module;

	ScopedPointer<BoolToggleUI> logIncomingUI;
	ScopedPointer<BoolToggleUI> logOutgoingUI;

	void resizedInternalHeader(Rectangle<int> &r) override; 

	virtual void resizedInternal(Rectangle<int> &) { }

	void controllableFeedbackAsyncUpdate(Controllable * c) override;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleEditor)
};



#endif  // MODULEEDITOR_H_INCLUDED
