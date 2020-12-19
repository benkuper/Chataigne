/*
  ==============================================================================

    IterativeStandardCondition.cpp
    Created: 19 Dec 2020 12:05:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "IterativeCondition.h"
#include "Common/Processor/Iterator/Iterator.h"

IterativeCondition::IterativeCondition(var params) :
	Condition(getTypeStringStatic(), params)
{
	sourceTarget = addTargetParameter("Input List", "This list that will ");
    sourceTarget->targetType = TargetParameter::CONTAINER;
    sourceTarget->showParentNameInEditor = false;
}

IterativeCondition::~IterativeCondition()
{
}

void IterativeCondition::setupIterator(IteratorProcessor* iterator)
{
    std::function<ControllableContainer* ()> getListFunc = std::bind(&IteratorProcessor::showAndGetList, iterator);

    sourceTarget->setRootContainer(&iterator->listManager);
    sourceTarget->customGetTargetContainerFunc = getListFunc;
    sourceTarget->hideInEditor = true;
}

void IterativeCondition::setValidState(int index, bool force)
{
}

var IterativeCondition::getJSONData()
{
    return var();
}

void IterativeCondition::loadJSONDataInternal(var data)
{
}

void IterativeCondition::setSourceList(BaseIteratorList* list)
{
}

void IterativeCondition::comparatorValidationChanged(BaseComparator*)
{
}

void IterativeCondition::forceCheck()
{
}

void IterativeCondition::onContainerParameterChangedInternal(Parameter* p)
{
}

void IterativeCondition::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
}

InspectableEditor* IterativeCondition::getEditor(bool isRoot)
{
    return nullptr;
}
