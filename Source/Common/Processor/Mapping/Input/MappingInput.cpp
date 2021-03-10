/*
  ==============================================================================

	MappingInput.cpp
	Created: 28 Oct 2016 8:11:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "MappingInput.h"
#include "Module/ModuleIncludes.h"
#include "ui/MappingInputEditor.h"

MappingInput::MappingInput(var params, Multiplex * processor) :
	BaseItem(getTypeStringStatic(params.getProperty("listMode", false)), false, false),
	MultiplexTarget(processor),
	multiplexListMode(params.getProperty("listMode", false)),
	inputReference(nullptr),
	list(nullptr),
	mappingInputAsyncNotifier(10)
{
	canBeCopiedAndPasted = true;

	nameCanBeChangedByUser = false;

	if (multiplexListMode)
	{
		inputTarget = addTargetParameter("Input List", "Multiplex list to be the input");
		inputTarget->targetType = TargetParameter::CONTAINER;
		inputTarget->setRootContainer(&multiplex->listManager);

		std::function<ControllableContainer* ()> getListFunc = std::bind(&Multiplex::showAndGetList, multiplex);
		inputTarget->customGetTargetContainerFunc = getListFunc;
		inputTarget->showParentNameInEditor = false;
	}
	else
	{
		inputTarget = addTargetParameter("Input Value", "Parameter to be the input");
		inputTarget->excludeTypesFilter.add(Trigger::getTypeStringStatic());

		inputTarget->customGetTargetFunc = &ModuleManager::getInstance()->showAllValuesAndGetControllable;
		inputTarget->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
		inputTarget->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;
	}
}



MappingInput::~MappingInput()
{
	clear();
	if(list != nullptr) list->removeListListener(this);
}

void MappingInput::lockInput(Parameter* input)
{
	setInput(input);
	if (input != nullptr) inputTarget->setEnabled(false);
}

void MappingInput::setInput(Parameter* _input)
{
	if (!inputReference.wasObjectDeleted() && inputReference != nullptr)
	{
		inputReference->removeParameterListener(this);

		Module* m = ControllableUtil::findParentAs<Module>(inputReference.get());
		if (m != nullptr) unregisterLinkedInspectable(m);
	}

	inputReference = _input;

	if (!inputReference.wasObjectDeleted() && inputReference != nullptr)
	{
		inputReference->addParameterListener(this);

		Module* m = ControllableUtil::findParentAs<Module>(inputReference.get());
		if (m != nullptr) registerLinkedInspectable(m);
	}

	mappinginputListeners.call(&MappingInput::Listener::inputReferenceChanged, this);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

void MappingInput::setList(BaseMultiplexList* newList)
{
	if (newList == list) return;

	if (list != nullptr)
	{
		list->removeListListener(this);
	}

	list = newList;

	if (list != nullptr)
	{
		list->addListListener(this);
	}

	mappinginputListeners.call(&MappingInput::Listener::inputReferenceChanged, this);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

void MappingInput::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (p == inputTarget && inputTarget->enabled)
	{
		if (!multiplexListMode) setInput(inputTarget->target.wasObjectDeleted() ? nullptr : dynamic_cast<Parameter*>(inputTarget->target.get()));
		else setList((BaseMultiplexList * )inputTarget->targetContainer.get());
	}
}

void MappingInput::onExternalParameterValueChanged(Parameter* p)
{
	if (p == inputReference && !multiplexListMode)
	{
		mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChanged, this, -1);
		mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::PARAMETER_VALUE_CHANGED, this, -1));
	}
}

void MappingInput::listReferenceUpdated()
{
	mappinginputListeners.call(&MappingInput::Listener::inputReferenceChanged, this);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

void MappingInput::listItemUpdated(int multiplexIndex)
{
	mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChanged, this, multiplexIndex);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::PARAMETER_VALUE_CHANGED, this, multiplexIndex));
}

Parameter* MappingInput::getInputAt(int multiplexIndex)
{
	if (!multiplexListMode) return inputReference.get();
	return dynamic_cast<Parameter*>(list != nullptr ? list->getTargetControllableAt(multiplexIndex) : nullptr);
}

void MappingInput::parameterRangeChanged(Parameter* p)
{
	ControllableContainer::parameterRangeChanged(p);
	mappinginputListeners.call(&MappingInput::Listener::inputParameterRangeChanged, this);
}

void MappingInput::clear()
{
	setInput(nullptr);
}

InspectableEditor* MappingInput::getEditor(bool isRoot)
{
	return new MappingInputEditor(this, isRoot);
}
