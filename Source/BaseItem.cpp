/*
  ==============================================================================

    BaseItem.cpp
    Created: 28 Oct 2016 8:04:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseItem.h"


BaseItem::BaseItem(const String &name) :
	ControllableContainer(name)
{
}

BaseItem::~BaseItem()
{
	clear();
}

void BaseItem::remove()
{
	baseItemListeners.call(&BaseItem::Listener::askForRemoveBaseItem, this);
}