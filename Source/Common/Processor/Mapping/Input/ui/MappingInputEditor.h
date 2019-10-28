/*
  ==============================================================================

    MappingInputUI.h
    Created: 28 Oct 2016 8:11:42pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MappingInput.h"

class MappingInputEditor :
	public GenericControllableContainerEditor,
	public MappingInput::AsyncListener
{
public:
	MappingInputEditor(MappingInput * input, bool isRoot);
	~MappingInputEditor();

	MappingInput * input;

	std::unique_ptr<ControllableUI> sourceFeedbackUI;

	void updateSourceUI();
	void resizedInternalHeader(Rectangle<int> &r) override;

	virtual void inputReferenceChangedAsync(MappingInput *);

protected:
	void newMessage(const MappingInput::MappingInputEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingInputEditor)
};