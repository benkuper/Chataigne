/*
  ==============================================================================

    MappingOutputManagerEditor.cpp
    Created: 31 May 2018 7:55:02am
    Author:  Ben

  ==============================================================================
*/

#include "MappingOutputManagerEditor.h"

MappingOutputManagerEditor::MappingOutputManagerEditor(MappingOutputManager * output, bool isRoot) :
	BaseCommandHandlerManagerEditor(output, CommandContext::MAPPING, isRoot),
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
		removeChildComponent(outUI);
		outUI = nullptr;
	}

	if (outputManager->outParam != nullptr)
	{
		outUI = (ParameterUI *)outputManager->outParam->createDefaultUI();
		addAndMakeVisible(outUI);
	}

	resized();
}

void MappingOutputManagerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	BaseCommandHandlerManagerEditor::resizedInternalHeader(r);
	if (outUI != nullptr) outUI->setBounds(r.removeFromRight(100).reduced(3));
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
