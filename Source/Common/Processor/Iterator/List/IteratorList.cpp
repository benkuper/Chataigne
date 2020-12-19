/*
  ==============================================================================

    IteratorList.cpp
    Created: 19 Dec 2020 12:00:35pm
    Author:  bkupe

  ==============================================================================
*/

#include "IteratorList.h"

BaseIteratorList::BaseIteratorList(const String& name, const String& listType, var params) :
    BaseItem(name, false),
    listSize(0),
    listType(listType)
{
    showInspectorOnSelect = false;
    isSelectable = false;
}

BaseIteratorList::~BaseIteratorList()
{
}

void BaseIteratorList::setSize(int size)
{
    if (size == listSize) return;
    listSize = size;
    updateControllablesSetup();
}


void BaseIteratorList::updateControllablesSetup()
{
    while (list.size() > listSize)
    {
        Controllable* c = list[list.size() - 1];
        list.removeAllInstancesOf(c);
        removeControllable(c);
    }

    while (list.size() < listSize)
    {
        Controllable* c = ControllableFactory::createControllable(listType);
        c->setNiceName("#" + String(list.size()+1));
        list.add(c);
        addControllable(c);
    }
}

var BaseIteratorList::getJSONData()
{
    var data = BaseItem::getJSONData();
    data.getDynamicObject()->setProperty("listSize", listSize);
    return data;
}

void BaseIteratorList::loadJSONData(var data, bool createIfNotThere)
{
    setSize(data.getProperty("listSize", 0));
    BaseItem::loadJSONData(data, createIfNotThere);
}

