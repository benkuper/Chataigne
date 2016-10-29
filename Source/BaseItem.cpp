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
	enabled = addBoolParameter("Enabled", "Enable / Disable this component", true);
	nameParam = addStringParameter("Name", "Name of the component", niceName);
}

BaseItem::~BaseItem()
{
	clear();
}

void BaseItem::remove()
{
	baseItemListeners.call(&BaseItem::Listener::askForRemoveBaseItem, this);
}

void BaseItem::onContainerParameterChanged(Parameter * p)
{
	if (p == nameParam)
	{
		setNiceName(nameParam->stringValue());
	}
}

void BaseItem::childAddressChanged(ControllableContainer *)
{
	DBG("here");
	nameParam->setValue(niceName);
}
