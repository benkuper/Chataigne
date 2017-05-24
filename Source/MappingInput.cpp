/*
  ==============================================================================

    MappingInput.cpp
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingInput.h"
#include "MappingInputEditor.h"

MappingInput::MappingInput() :
	ControllableContainer("Input"),
	inputReference(nullptr)
{
	nameCanBeChangedByUser = false;
	inputTarget = addTargetParameter("Input", "Parameter to be the input");
}

MappingInput::~MappingInput()
{
	setInput(nullptr);
}

void MappingInput::lockInput(Parameter * input)
{
	setInput(input);
	if(input != nullptr) inputTarget->setEnabled(false);
}

void MappingInput::setInput(Parameter * _input)
{
	if (inputReference != nullptr)
	{
		inputReference->removeParameterListener(this);
	}
	
	inputReference = _input;

	if (inputReference != nullptr)
	{
		inputReference->addParameterListener(this);
	}

	mappinginputListeners.call(&MappingInput::Listener::inputReferenceChanged, this);
}

void MappingInput::onContainerParameterChanged(Parameter * p)
{
	if (p == inputTarget)
	{
		setInput(inputTarget->target.wasObjectDeleted() ? nullptr : dynamic_cast<Parameter *>(inputTarget->target.get()));
	}
}

void MappingInput::onExternalParameterChanged(Parameter * p)
{
	if (p == inputReference)
	{
		mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChanged, this);
	}
}

InspectableEditor * MappingInput::getEditor(bool isRoot)
{
	return new MappingInputEditor(this,isRoot);
}
