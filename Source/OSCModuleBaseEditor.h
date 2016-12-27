/*
  ==============================================================================

    OSCModuleBaseEditor.h
    Created: 27 Dec 2016 12:40:08pm
    Author:  Ben

  ==============================================================================
*/

#ifndef OSCMODULEBASEEDITOR_H_INCLUDED
#define OSCMODULEBASEEDITOR_H_INCLUDED

#include "OSCModule.h"
#include "IntStepperUI.h"
#include "BoolToggleUI.h"
#include "StringParameterUI.h"

class OSCModuleBaseEditor :
	public InspectableEditor,
	public ControllableContainer::ContainerAsyncListener
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


	virtual void resized();
	virtual int resizedInternal(Rectangle<int> r) { return 0; } //to be overriden by child classes

	void newMessage(const ContainerAsyncEvent &e) override;
	virtual void controllableFeedbackAsyncUpdate(Controllable * c);
};



#endif  // OSCMODULEBASEEDITOR_H_INCLUDED
