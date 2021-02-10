/*
  ==============================================================================

    MultiplexList.cpp
    Created: 19 Dec 2020 12:00:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "MultiplexList.h"
#include "Module/ModuleManager.h"

BaseMultiplexList::BaseMultiplexList(const String& name, var params) :
    BaseItem(name, false),
    listSize(0)
{
    showInspectorOnSelect = false;
    isSelectable = false;
}

BaseMultiplexList::~BaseMultiplexList()
{
}

void BaseMultiplexList::setSize(int size)
{
    if (size == listSize) return;
    listSize = size;
    updateControllablesSetup();
}

void BaseMultiplexList::updateControllablesSetup()
{
    while (list.size() > listSize)
    {
        Controllable* c = list[list.size() - 1];
        list.removeAllInstancesOf(c);
        removeControllable(c);
    }

    while (list.size() < listSize)
    {
        Controllable* c = ControllableFactory::createControllable(getTypeString());
        c->isCustomizableByUser = true;
        c->setNiceName("#" + String(list.size() + 1));
        list.add(c);
        addControllable(c);
    }
}

var BaseMultiplexList::getJSONData()
{
    var data = BaseItem::getJSONData();
    data.getDynamicObject()->setProperty("listSize", listSize);
    return data;
}

void BaseMultiplexList::loadJSONData(var data, bool createIfNotThere)
{
    setSize(data.getProperty("listSize", 0));
    BaseItem::loadJSONData(data, createIfNotThere);
}

void BaseMultiplexList::notifyItemUpdated(int multiplexIndex)
{
    jassert(multiplexIndex >= 0);
    listListeners.call(&ListListener::listItemUpdated, multiplexIndex);
}


InputValueMultiplexList::InputValueMultiplexList(var params) :
    BaseMultiplexList(getTypeString(), params)
{
}

InputValueMultiplexList::~InputValueMultiplexList()
{
    for (auto& c : inputControllables)
    {
        if (c == nullptr || c.wasObjectDeleted()) continue;
        if (c->type == c->TRIGGER) ((Trigger*)c.get())->removeTriggerListener(this);
        else((Parameter*)c.get())->removeParameterListener(this);
    }
}

void InputValueMultiplexList::updateControllablesSetup()
{
    while (list.size() > listSize)
    {
        int index = list.size() - 1;
        Controllable* c = list[index];
        list.removeAllInstancesOf(c);
        removeControllable(c);
        if (Controllable * c = inputControllables[index])
        {
            if (c->type == c->TRIGGER) ((Trigger*)c)->removeTriggerListener(this);
            else((Parameter*)c)->removeParameterListener(this);
        }
    }

    while (list.size() < listSize)
    {
        TargetParameter* tp = addTargetParameter("#" + String(list.size() + 1), "Input Value Target");
        tp->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
        tp->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
        tp->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;

        list.add(tp);
        inputControllables.add(nullptr);
    }
}

void InputValueMultiplexList::onContainerParameterChangedInternal(Parameter* p)
{
    int index = list.indexOf(p);
    if (index != -1)
    {
        if (Controllable* c = inputControllables[index])
        {
            if (c->type == c->TRIGGER) ((Trigger*)c)->removeTriggerListener(this);
            else((Parameter*)c)->removeParameterListener(this);
        }

        inputControllables.set(index, nullptr);

        if (Controllable* c = ((TargetParameter*)p)->target)
        {
            if (c->type == c->TRIGGER) ((Trigger*)c)->addTriggerListener(this);
            else((Parameter*)c)->addParameterListener(this);
            inputControllables.set(index, c);

            if(index == 0) listListeners.call(&ListListener::listReferenceUpdated);
            notifyItemUpdated(index);
        }
    }

}

void InputValueMultiplexList::onExternalParameterRangeChanged(Parameter* p)
{
    if (inputControllables.indexOf(p) == 0) listListeners.call(&ListListener::listReferenceUpdated);
}

void InputValueMultiplexList::onExternalParameterValueChanged(Parameter* p)
{
    notifyItemUpdated(inputControllables.indexOf(p));
}

void InputValueMultiplexList::onExternalTriggerTriggered(Trigger* t)
{
    notifyItemUpdated(inputControllables.indexOf(t));
}
