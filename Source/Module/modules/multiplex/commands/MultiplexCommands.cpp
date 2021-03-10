/*
  ==============================================================================

    GenericMultiplexCommands.cpp
    Created: 19 Feb 2021 4:54:36pm
    Author:  bkupe

  ==============================================================================
*/

MultiplexCommand::MultiplexCommand(MultiplexModule* _module, CommandContext context, var params, Multiplex* multiplex) :
    BaseCommand(_module, context, params, multiplex),
    value(nullptr),
    listTarget(nullptr)
{
    type = (Type)(int)(params.getProperty("type", SET_LIST_VALUE));

    if (isMultiplexed())
    {
        listTarget = addTargetParameter("List", "The list to set the value", &multiplex->listManager );
        listTarget->targetType = TargetParameter::CONTAINER;
        std::function<ControllableContainer* ()> getListFunc = std::bind(&Multiplex::showAndGetList, multiplex);
        listTarget->customGetTargetContainerFunc = getListFunc;
        listTarget->showParentNameInEditor = false;
    }
   
}

MultiplexCommand::~MultiplexCommand()
{
}


void MultiplexCommand::onContainerParameterChanged(Parameter* p)
{
    if (p == listTarget) updateValueParameter();
}

void MultiplexCommand::updateValueParameter()
{
    if (value != nullptr)
    {
        removeControllable(value);
        value = nullptr;
    }
    
    if (listTarget == nullptr) return;

    BaseMultiplexList* list = dynamic_cast<BaseMultiplexList *>(listTarget->targetContainer.get());
    if (list == nullptr) return;

    Parameter* firstParam = dynamic_cast<Parameter*>(list->list[0]);
    if (firstParam == nullptr) return;

    value = ControllableFactory::createParameterFrom(firstParam);
    if (value == nullptr) return;

    addParameter(value);

    linkParamToMappingIndex(value, 0);
}


void MultiplexCommand::triggerInternal(int multiplexIndex)
{
    if (value == nullptr || !isMultiplexed()) return;
   
    BaseMultiplexList* list = dynamic_cast<BaseMultiplexList*>(listTarget->targetContainer.get());
    if (list == nullptr) return; 
    
    if (Parameter* p = dynamic_cast<Parameter*>(list->list[multiplexIndex])) 
    {
        p->setValue(getLinkedValue(value, multiplexIndex));
    }
}
