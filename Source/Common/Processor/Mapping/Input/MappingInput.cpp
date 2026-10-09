/*
  ==============================================================================

	MappingInput.cpp
	Created: 28 Oct 2016 8:11:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"


MappingInput::MappingInput(const String& name, var params, Multiplex* processor) :
	BaseItem(name, false, false),
	MultiplexTarget(processor),
	inputReference(nullptr),
	mappingInputAsyncNotifier(10)
{
	canBeCopiedAndPasted = true;
	nameCanBeChangedByUser = false;

	triggersProcess = addBoolParameter("Triggers Process", "If true, the input will trigger the process when its value changes", true);
	triggersProcess->hideInEditor = true;
}

MappingInput::~MappingInput()
{
	clear();
}

void MappingInput::clear()
{
	setInput(nullptr);
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
	inputValueSnapshot.reset(_input != nullptr ? ControllableFactory::createParameterFrom(_input, true, true) : nullptr);

	if (!inputReference.wasObjectDeleted() && inputReference != nullptr)
	{
		inputReference->addParameterListener(this);

		Module* m = ControllableUtil::findParentAs<Module>(inputReference.get());
		if (m != nullptr) registerLinkedInspectable(m);
	}

	mappinginputListeners.call(&MappingInput::Listener::inputReferenceChanged, this, -1);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}


Parameter* MappingInput::getInputAt(int multiplexIndex)
{
	return inputReference.get();
}

void MappingInput::parameterRangeChanged(Parameter* p)
{
	if (p == inputReference && inputValueSnapshot != nullptr)
		inputValueSnapshot->setRange(p->minimumValue, p->maximumValue);
	ControllableContainer::parameterRangeChanged(p);
	mappinginputListeners.call(&MappingInput::Listener::inputParameterRangeChanged, this);
}

void MappingInput::parameterValueChangedWithValue(Parameter* p, const var& value)
{
	if (p == inputReference && inputValueSnapshot != nullptr
		&& p->type != Controllable::ENUM && p->type != Controllable::TARGET)
	{
		// Worker-thread notifications may wait behind several newer values. Keep
		// the source untouched and process the value captured for this event.
		// Enum notifications contain option data rather than the selected key.
		inputValueSnapshot->setValue(value, true, true);
		ScopedValueSetter<bool> delivering(deliveringCapturedValue, true);
		BaseItem::parameterValueChanged(p);
		return;
	}
	BaseItem::parameterValueChanged(p);
}

void MappingInput::onExternalParameterValueChanged(Parameter* p)
{
	if (p == inputReference)
	{
		if (deliveringCapturedValue)
			mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChangedWithValue, this, -1, inputValueSnapshot.get());
		else mappinginputListeners.call(&MappingInput::Listener::inputParameterValueChanged, this, -1);
		mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::PARAMETER_VALUE_CHANGED, this, -1));
	}
}

// STANDARD

StandardMappingInput::StandardMappingInput(var params, Multiplex* processor) :
	MappingInput(getTypeStringStatic(params.getProperty("listMode", false)), params, processor),
	multiplexListMode(params.getProperty("listMode", false)),
	list(nullptr)
{

	if (multiplexListMode)
	{
		inputTarget = addTargetParameter("Input List", "Multiplex list to be the input");
		inputTarget->targetType = TargetParameter::CONTAINER;
		inputTarget->setRootContainer(&multiplex->listManager);

		std::function<void(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)>)> getListFunc = std::bind(&Multiplex::showAndGetList, multiplex, std::placeholders::_1, std::placeholders::_2);
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



StandardMappingInput::~StandardMappingInput()
{
	if (list != nullptr && !list->isClearing) list->removeMultiplexListListener(this);
}

void StandardMappingInput::lockInput(Parameter* input)
{
	setInput(input);
	if (input != nullptr && (list == nullptr || !list->isClearing)) inputTarget->setEnabled(false);
}


void StandardMappingInput::setList(BaseMultiplexList* newList)
{
	if (newList == list) return;

	if (list != nullptr && !list->isClearing)
	{
		list->removeMultiplexListListener(this);
	}

	list = newList;

	if (list != nullptr && !list->isClearing)
	{
		list->addMultiplexListListener(this);
	}

	mappinginputListeners.call(&StandardMappingInput::Listener::inputReferenceChanged, this, -1);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

Parameter* StandardMappingInput::getInputAt(int multiplexIndex)
{
	if (!multiplexListMode) return MappingInput::getInputAt(multiplexIndex);
	return dynamic_cast<Parameter*>(list != nullptr && !list->isClearing ? list->getTargetControllableAt(multiplexIndex) : nullptr);
}


void StandardMappingInput::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (p == inputTarget && inputTarget->enabled)
	{
		if (!multiplexListMode) setInput(inputTarget->target.wasObjectDeleted() ? nullptr : dynamic_cast<Parameter*>(inputTarget->target.get()));
		else setList((BaseMultiplexList*)inputTarget->targetContainer.get());
	}
}

void StandardMappingInput::onExternalParameterValueChanged(Parameter* p)
{
	if (p == inputReference && multiplexListMode) return; // only call external if not in list mode
	MappingInput::onExternalParameterValueChanged(p);
}

void StandardMappingInput::listReferenceUpdated(int multiplexIndex)
{
	mappinginputListeners.call(&StandardMappingInput::Listener::inputReferenceChanged, this, multiplexIndex);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_REFERENCE_CHANGED, this));
}

void StandardMappingInput::listItemUpdated(int multiplexIndex)
{
	mappinginputListeners.call(&StandardMappingInput::Listener::inputParameterValueChanged, this, multiplexIndex);
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::PARAMETER_VALUE_CHANGED, this, multiplexIndex));
}



void StandardMappingInput::multiplexPreviewIndexChanged()
{
	mappingInputAsyncNotifier.addMessage(new MappingInputEvent(MappingInputEvent::INPUT_PREVIEW_CHANGED, this, getPreviewIndex()));
}

InspectableEditor* StandardMappingInput::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new MappingInputEditor(this, isRoot);
}


// MANUAL


ManualMappingInput::ManualMappingInput(var params, Multiplex* multiplex) :
	MappingInput(getTypeStringStatic(params.getProperty("paramType", "").toString()), params, multiplex)
{
	manualParam = (Parameter*)ControllableFactory::createControllable(params.getProperty("paramType", ""));
	manualParam->isCustomizableByUser = true;
	addParameter(manualParam);
	setInput(manualParam);
}

ManualMappingInput::~ManualMappingInput()
{
}

void ManualMappingInput::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == manualParam) onExternalParameterValueChanged(p);
	MappingInput::onContainerParameterChangedInternal(p);
}

void ManualMappingInput::parameterRangeChanged(Parameter* p)
{
	if (p == manualParam) onExternalParameterRangeChanged(p);
	MappingInput::parameterRangeChanged(p);
}
