/*
  ==============================================================================

    CustomOSCCommandArgumentManager.cpp
    Created: 22 Feb 2017 8:51:39am
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandArgumentManager.h"
#include "ControllableFactory.h"
#include "CustomOSCCommandArgumentManagerEditor.h"

CustomOSCCommandArgumentManager::CustomOSCCommandArgumentManager(bool _mappingEnabled) :
	BaseManager("arguments"),
	mappingEnabled(_mappingEnabled)
{
	selectItemWhenCreated = false;
}

void CustomOSCCommandArgumentManager::addItemWithParam(Parameter * p, var data, bool fromUndoableAction)
{
	CustomOSCCommandArgument * a = new CustomOSCCommandArgument("#" + String(items.size() + 1), p,mappingEnabled);
	a->addArgumentListener(this);
	addItem(a, data, fromUndoableAction);
}

void CustomOSCCommandArgumentManager::addItemFromType(Parameter::Type type, var data, bool fromUndoableAction)
{
	Parameter * p = nullptr;
	String id = String(items.size() + 1);

	switch (type)
	{
	case Parameter::STRING:
		p = new StringParameter("#" + id, "Argument #" + id + ", type int", "myString");
		break;
	case Parameter::FLOAT:
		p = new FloatParameter("#" + id, "Argument #" + id + ", type foat", 0, 0, 1);
		break;
	case Parameter::INT:
		p = new IntParameter("#" + id, "Argument #" + id + ", type int", 0, -1000, 1000);
		break;
	case Parameter::BOOL:
		p = new BoolParameter("#" + id, "Argument #" + id + ", type bool", false);
		break;
        default:
            break;
	}

	jassert(p != nullptr);
	addItemWithParam(p, data, fromUndoableAction);
}

void CustomOSCCommandArgumentManager::addItemFromData(var data, bool fromUndoableAction)
{
	String s = data.getProperty("type", "");
	DBG("add item from data : " << s);
	if (s.isEmpty()) return;
	addItemWithParam((Parameter *)ControllableFactory::createControllable(s), data, fromUndoableAction);
}

void CustomOSCCommandArgumentManager::autoRenameItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->niceName.startsWithChar('#')) items[i]->setNiceName("#" + String(i + 1));
	}
}

void CustomOSCCommandArgumentManager::removeItemInternal(CustomOSCCommandArgument *)
{
	autoRenameItems();
}

void CustomOSCCommandArgumentManager::useForMappingChanged(CustomOSCCommandArgument * i)
{
	/*
	if (i->useForMapping->boolValue())
	{
		for (auto &it : items) if (it != i) it->useForMapping->setValue(false);
	}
	*/

	argumentManagerListeners.call(&ManagerListener::useForMappingChanged, i);
}

InspectableEditor * CustomOSCCommandArgumentManager::getEditor(bool isRoot)
{
	return new CustomOSCCommandArgumentManagerEditor(this, isRoot);
}
