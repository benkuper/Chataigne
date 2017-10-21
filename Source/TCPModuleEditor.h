/*
  ==============================================================================

    TCPModuleEditor.h
    Created: 21 Oct 2017 5:50:06pm
    Author:  Ben

  ==============================================================================
*/

#pragma once


#include "ModuleEditor.h"
#include "TCPModule.h"


class TCPModuleEditor :
	public ModuleEditor
{
public:
	TCPModuleEditor(TCPModule * TCPModule, bool isRoot);
	virtual ~TCPModuleEditor();

	TCPModule * tcpModule;

	Label receiveLabel;
	ScopedPointer<IntStepperUI> localPortUI;
	ScopedPointer<BoolToggleUI> isConnectedUI;

	Label sendLabel;
	ScopedPointer<StringParameterUI> remoteHostUI;
	ScopedPointer<IntStepperUI> remotePortUI;
	ScopedPointer<BoolToggleUI> useLocalUI;

	virtual void resizedInternalContent(Rectangle<int> &r) override;
	virtual void resizedTCPInternalContent(Rectangle<int> &) {} //to be overriden by child classes

	virtual void controllableFeedbackAsyncUpdate(Controllable * c) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TCPModuleEditor)
};

