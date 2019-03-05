/*
  ==============================================================================

    MappingInput.cpp
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingInput.h"
#include "Module/ModuleManager.h"
#include "ui/MappingInputEditor.h"

MappingInput::MappingInput() :
	ControllableContainer("Input"),
	inputReference(nullptr),
	mappingInputAsyncNotifier(10)
{
	
	nameCanBeChangedByUser = false;
	inputTarget = addTargetParameter("Input Value", "Parameter to be the input");
	inputTarget->showTriggers = false;
	inputTarget->customGetTargetFunc = &ModuleManager::getInstance()->showAllValuesAndGetControllable;
	inputTarget->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable; 
	inputTarget->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;
}

MappingInput::~MappingInput()
{
	clear();
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
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

void MappingInput::onContainerParameterChanged(Parameter * p)
{
	if (p == inputTarget)
	{
		setInput(inputTarget->target.wasObjectDeleted() ? nullptr : dynamic_cast<Parameter *>(inputTarget->target.get()));
	}
}

void MappingInput::onExternalParameterValueChanged(Parameter * p)
{
	if (p == inputReference)
	{
		mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChanged, this);
		mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::PARAMETER_VALUE_CHANGED, this));
	}
}

void MappingInput::parameterRangeChanged(Parameter * p)
{
	ControllableContainer::parameterRangeChanged(p);
	mappinginputListeners.call(&MappingInput::Listener::inputParameterRangeChanged, this);
}

void MappingInput::clear()
{
	setInput(nullptr);
}

InspectableEditor * MappingInput::getEditor(bool isRoot)
{
	return new MappingInputEditor(this, isRoot);
}
