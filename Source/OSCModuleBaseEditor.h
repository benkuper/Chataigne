/*
  ==============================================================================

    OSCModuleBaseEditor.h
    Created: 27 Dec 2016 12:40:08pm
    Author:  Ben

  ==============================================================================
*/

#ifndef OSCMODULEBASEEDITOR_H_INCLUDED
#define OSCMODULEBASEEDITOR_H_INCLUDED

#include "ModuleEditor.h"
#include "OSCModule.h"
#include "IntStepperUI.h"


class OSCModuleBaseEditor :
	public ModuleEditor
{
public:
	OSCModuleBaseEditor(OSCModule * oscModule, bool isRoot);
	virtual ~OSCModuleBaseEditor();

	OSCModule * oscModule;

	Label receiveLabel;
	ScopedPointer<IntStepperUI> localPortUI;
	ScopedPointer<BoolToggleUI> isConnectedUI;

	Label sendLabel;
	ScopedPointer<StringParameterUI> remoteHostUI;
	ScopedPointer<IntStepperUI> remotePortUI;
	ScopedPointer<BoolToggleUI> useLocalUI;

	virtual void resizedInternalContent(Rectangle<int> &r);
	virtual void resizedOSCInternalContent(Rectangle<int> &) {} //to be overriden by child classes

	virtual void controllableFeedbackAsyncUpdate(Controllable * c) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCModuleBaseEditor)
};



#endif  // OSCMODULEBASEEDITOR_H_INCLUDED
