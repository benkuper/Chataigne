/*
  ==============================================================================

    Iterator.cpp
    Created: 17 Dec 2020 5:12:36pm
    Author:  bkupe

  ==============================================================================
*/

#include "Iterator.h"
#include "ui/IteratorUI.h"

IteratorProcessor::IteratorProcessor(var params) :
    Processor(getTypeString()),
    listManager(this),
    processorManager("Processors", this)
{
    count = addIntParameter("Count", "The number of items for each list of this iterator", 1, 0);

    addChildControllableContainer(&listManager);

    processorManager.hideInEditor = true;
    addChildControllableContainer(&processorManager);

    saveAndLoadRecursiveData = true;
}

IteratorProcessor::~IteratorProcessor()
{
}

void IteratorProcessor::onContainerParameterChangedInternal(Parameter* p)
{
    Processor::onContainerParameterChangedInternal(p);
    if (p == count)
    {
        for (auto& l : listManager.items) l->setSize(count->intValue());
        iteratorListeners.call(&IteratorListener::iteratorCountChanged);
    }
}

void IteratorProcessor::updateDisables(bool force)
{
    processorManager.setForceDisabled(!enabled->boolValue() || forceDisabled);
}

BaseIteratorList* IteratorProcessor::showAndGetList()
{
    PopupMenu p;
    for (int i = 0; i < listManager.items.size(); i++) p.addItem(i + 1, listManager.items[i]->niceName);

    if (int result = p.show()) return listManager.items[result - 1];
    return nullptr;
}

ProcessorUI* IteratorProcessor::getUI()
{
    return new IteratorUI(this);
}

IterativeTarget::IterativeTarget(IteratorProcessor* it) :
    iterator(it)
{
    if(iterator != nullptr) iterator->addIteratorListener(this);
}

IterativeTarget::~IterativeTarget()
{
    if(iterator != nullptr) iterator->removeIteratorListener(this);
}

bool IterativeTarget::isIterative() const
{
    return iterator != nullptr;
}

int IterativeTarget::getIterationCount() const
{
    return isIterative() ? iterator->count->intValue() : 1;
}
