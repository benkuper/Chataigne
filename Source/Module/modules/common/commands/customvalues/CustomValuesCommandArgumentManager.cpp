/*
  ==============================================================================

    CustomValuesCommandArgumentManager.cpp
    Created: 22 Feb 2017 8:51:39am
    Author:  Ben

  ==============================================================================
*/

#include "CustomValuesCommandArgumentManager.h"
#include "ui/CustomValuesCommandArgumentManagerEditor.h"

CustomValuesCommandArgumentManager::CustomValuesCommandArgumentManager(bool _mappingEnabled) :
	BaseManager("arguments"),
	mappingEnabled(_mappingEnabled)
{
	selectItemWhenCreated = false;
	editorCanBeCollapsed = false;
	
}

CustomValuesCommandArgument * CustomValuesCommandArgumentManager::addItemWithParam(Parameter * p, var data, bool fromUndoableAction)
{
	CustomValuesCommandArgument * a = new CustomValuesCommandArgument("#" + String(items.size() + 1), p,mappingEnabled);
	a->addArgumentListener(this);
	if (mappingEnabled && items.size() == 1) a->useForMapping->setValue(true); 
	addItem(a, data, fromUndoableAction);
	return a;
}

CustomValuesCommandArgument * CustomValuesCommandArgumentManager::addItemFromType(Parameter::Type type, var data, bool fromUndoableAction)
{
	Parameter * p = nullptr;
	String id = String(items.size() + 1);

	switch (type)
	{
	case Parameter::STRING:
		p = new StringParameter("#" + id, "Argument #" + id + ", type int", "example");
		break;
	case Parameter::FLOAT:
		p = new FloatParameter("#" + id, "Argument #" + id + ", type foat", 0);
		break;
	case Parameter::INT:
		p = new IntParameter("#" + id, "Argument #" + id + ", type int", 0);
		break;
	case Parameter::BOOL:
		p = new BoolParameter("#" + id, "Argument #" + id + ", type bool", false);
		break;
	case Parameter::COLOR:
		p = new ColorParameter("#" + id, "Argument #" + id + ", type color");
		break;
	case Parameter::POINT2D:
		p = new Point2DParameter("#" + id, "Argument #" + id + ", type 2D");
		break;
	case Parameter::POINT3D:
		p = new Point3DParameter("#" + id, "Argument #" + id + ", type 3D");
		break;

        default:
            break;
	}

	if (p == nullptr) return nullptr;
	p->isCustomizableByUser = true;
	return addItemWithParam(p, data, fromUndoableAction);
}

CustomValuesCommandArgument * CustomValuesCommandArgumentManager::addItemFromData(var data, bool fromUndoableAction)
{
	String s = data.getProperty("type", "");
	if (s.isEmpty()) return nullptr;
	Parameter * p = (Parameter *)ControllableFactory::createControllable(s);
	if (p == nullptr)
	{
		LOG("Error loading custom argument !");
		return nullptr;
	}

 	return addItemWithParam(p, data, fromUndoableAction);
}

void CustomValuesCommandArgumentManager::autoRenameItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->niceName.startsWithChar('#')) items[i]->setNiceName("#" + String(i + 1));
	}
}

void CustomValuesCommandArgumentManager::removeItemInternal(CustomValuesCommandArgument * i)
{
	autoRenameItems();
	useForMappingChanged(i);
}

void CustomValuesCommandArgumentManager::useForMappingChanged(CustomValuesCommandArgument * i)
{
	/*
	if (i->useForMapping->boolValue())
	{
		for (auto &it : items) if (it != i) it->useForMapping->setValue(false);
	}
	*/

	argumentManagerListeners.call(&ManagerListener::useForMappingChanged, i);
}

InspectableEditor * CustomValuesCommandArgumentManager::getEditor(bool isRoot)
{
	return new CustomValuesCommandArgumentManagerEditor(this, isRoot);
}
