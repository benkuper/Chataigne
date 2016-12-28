/*
  ==============================================================================

    ModuleEditor.h
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEEDITOR_H_INCLUDED
#define MODULEEDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "Module.h"
#include "BoolToggleUI.h"
#include "StringParameterUI.h"

class ModuleEditor :
	public InspectableEditor,
	public ControllableContainer::ContainerAsyncListener
{
public:
	ModuleEditor(Module * module, bool isRoot);
	virtual ~ModuleEditor();

	Module * module;

	
	ScopedPointer<BoolToggleUI> enabledUI;
	ScopedPointer<StringParameterUI> nameUI;
	ScopedPointer<BoolToggleUI> logIncomingUI;
	ScopedPointer<BoolToggleUI> logOutgoingUI;
	
	const int headerHeight = 20;

	void paint(Graphics &g) override;
	void resized() override; 
	virtual void resizedInternal(Rectangle<int> &) { }

	void newMessage(const ContainerAsyncEvent &e) override;
	virtual void controllableFeedbackAsyncUpdate(Controllable * c);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleEditor)
};



#endif  // MODULEEDITOR_H_INCLUDED
