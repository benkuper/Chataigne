/*
  ==============================================================================

    GenericMultiplexCommands.cpp
    Created: 19 Feb 2021 4:54:36pm
    Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "MultiplexCommands.h"

MultiplexCommand::MultiplexCommand(MultiplexModule* _module, CommandContext context, var params, Multiplex* multiplex) :
    GenericControllableCommand(_module, context, params, multiplex)
{
   // type = (Type)(int)(params.getProperty("type", SET_LIST_VALUE));

    if (isMultiplexed())
    {
        target->targetType = TargetParameter::CONTAINER;
        std::function<ControllableContainer* ()> getListFunc = std::bind(&Multiplex::showAndGetList, multiplex);
        target->customGetTargetContainerFunc = getListFunc;
        target->showParentNameInEditor = false;
    }
   
}

MultiplexCommand::~MultiplexCommand()
{
}


Controllable* MultiplexCommand::getControllableFromTarget()
{
    BaseMultiplexList* list = dynamic_cast<BaseMultiplexList*>(target->targetContainer.get());
    if (list == nullptr) return nullptr;

    return list->list[0];
}

Controllable* MultiplexCommand::getTargetControllableAtIndex(int multiplexIndex)
{
    if (BaseMultiplexList* list = dynamic_cast<BaseMultiplexList*>(target->targetContainer.get()))
    {
        if (InputValueMultiplexList* il = dynamic_cast<InputValueMultiplexList *>(list))
        {
            return il->inputControllables[multiplexIndex];
        }

        return list->list[multiplexIndex];
    }

    return nullptr;
}