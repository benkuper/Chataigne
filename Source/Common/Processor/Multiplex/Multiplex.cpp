/*
  ==============================================================================

    Multiplex.cpp
    Created: 17 Dec 2020 5:12:36pm
    Author:  bkupe

  ==============================================================================
*/

#include "Multiplex.h"
#include "ui/MultiplexUI.h"

Multiplex::Multiplex(var params) :
    Processor(getTypeString()),
    listManager(this),
    processorManager("Processors", this)
{
    count = addIntParameter("Count", "The number of items for each list of this multiplex", 1, 0);

    addChildControllableContainer(&listManager);

    processorManager.hideInEditor = true;
    addChildControllableContainer(&processorManager);

    saveAndLoadRecursiveData = true;
}

Multiplex::~Multiplex()
{
}

void Multiplex::onContainerParameterChangedInternal(Parameter* p)
{
    Processor::onContainerParameterChangedInternal(p);
    if (p == count)
    {
        for (auto& l : listManager.items) l->setSize(count->intValue());
        multiplexListeners.call(&MultiplexListener::multiplexCountChanged);
    }
}

void Multiplex::updateDisables(bool force)
{
    processorManager.setForceDisabled(!enabled->boolValue() || forceDisabled);
}

BaseMultiplexList* Multiplex::showAndGetList()
{
    PopupMenu p;
    for (int i = 0; i < listManager.items.size(); i++) p.addItem(i + 1, listManager.items[i]->niceName);

    if (int result = p.show()) return listManager.items[result - 1];
    return nullptr;
}

ProcessorUI* Multiplex::getUI()
{
    return new MultiplexUI(this);
}

MultiplexTarget::MultiplexTarget(Multiplex* mp) :
    multiplex(mp)
{
    if(multiplex != nullptr) multiplex->addMultiplexListener(this);
}

MultiplexTarget::~MultiplexTarget()
{
    if(multiplex != nullptr) multiplex->removeMultiplexListener(this);
}

bool MultiplexTarget::isMultiplexed() const
{
    return multiplex != nullptr;
}

int MultiplexTarget::getMultiplexCount() const
{
    return isMultiplexed() ? multiplex->count->intValue() : 1;
}
