/*
  ==============================================================================

    MappingOutputManagerEditor.cpp
    Created: 31 May 2018 7:55:02am
    Author:  Ben

  ==============================================================================
*/

#include "MappingOutputManagerEditor.h"

MappingOutputManagerEditor::MappingOutputManagerEditor(MappingOutputManager * output, bool isRoot) :
	BaseCommandHandlerManagerEditor(output, CommandContext::MAPPING, isRoot, output->isMultiplexed()),
	outputManager(output)
{
	outputManager->addAsyncOutputManagerListener(this);
	updateOutputUI();
}

MappingOutputManagerEditor::~MappingOutputManagerEditor()
{
	if (!inspectable.wasObjectDeleted()) outputManager->removeAsyncOutputManagerListener(this);
}

void MappingOutputManagerEditor::updateOutputUI()
{
	if (outUI != nullptr)
	{
		removeChildComponent(outUI.get());
		outUI = nullptr;
	}


	int pi = outputManager->getPreviewIndex();
	Array<WeakReference<Parameter>> outParams = outputManager->outParams[pi];

	if (outParams.size() > 0 && outParams[0] != nullptr)
	{
		outUI.reset((ParameterUI *)outParams[0]->createDefaultUI());
		outUI->showLabel = false;
		addAndMakeVisible(outUI.get());
	}

	resized();
}

void MappingOutputManagerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	BaseCommandHandlerManagerEditor::resizedInternalHeader(r);
	if (outUI != nullptr) outUI->setBounds(r.removeFromRight(140).reduced(3));
}

void MappingOutputManagerEditor::newMessage(const MappingOutputManager::OutputManagerEvent & e)
{
	switch (e.type)
	{
	case MappingOutputManager::OutputManagerEvent::OUTPUT_CHANGED:
		updateOutputUI();
		break;
	}
}
