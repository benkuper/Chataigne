/*
  ==============================================================================

    MappingInputUI.h
    Created: 28 Oct 2016 8:11:42pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGINPUTUI_H_INCLUDED
#define MAPPINGINPUTUI_H_INCLUDED

#include "MappingInput.h"

class MappingInputEditor :
	public GenericControllableContainerEditor,
	public MappingInput::Listener
{
public:
	MappingInputEditor(MappingInput * input, bool isRoot);
	~MappingInputEditor();

	MappingInput * input;

	ScopedPointer<ControllableUI> sourceFeedbackUI;

	void updateSourceUI();
	void resizedInternalHeader(Rectangle<int> &r) override;

	void inputReferenceChanged(MappingInput *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingInputEditor)
};


#endif  // MAPPINGINPUTUI_H_INCLUDED
