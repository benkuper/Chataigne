/*
  ==============================================================================

	CustomValuesCommandArgumentManager.cpp
	Created: 22 Feb 2017 8:51:39am
	Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "Common/Command/CommandIncludes.h"

CustomValuesCommandArgumentManager::CustomValuesCommandArgumentManager(const String& name, bool _mappingEnabled, bool templateMode, Multiplex* multiplex) :
	BaseManager(name),
	MultiplexTarget(multiplex),
	isBeingDestroyed(false),
	mappingEnabled(_mappingEnabled),
	templateMode(templateMode),
	enablePrecison(true),
	linkedTemplateManager(nullptr),
	createParamCallbackFunc(nullptr)
{
	selectItemWhenCreated = false;
	editorCanBeCollapsed = false;

	scriptObject.getDynamicObject()->setMethod("addItem", &CustomValuesCommandArgumentManager::addItemWithTypeFromScript);
}

CustomValuesCommandArgumentManager::~CustomValuesCommandArgumentManager()
{
	isBeingDestroyed = true;
	linkToTemplate(nullptr);
}

void CustomValuesCommandArgumentManager::linkToTemplate(CustomValuesCommandArgumentManager* t)
{
	if (linkedTemplateManager != nullptr && !linkedTemplateManagerRef.wasObjectDeleted())
	{
		if (linkedTemplateManager != nullptr)
		{
			linkedTemplateManager->removeBaseManagerListener(this);
		}
		linkedTemplateManager = nullptr;
	}

	linkedTemplateManager = t;
	linkedTemplateManagerRef = t;

	if (linkedTemplateManager != nullptr)
	{
		linkedTemplateManager->addBaseManagerListener(this);
		rebuildFromTemplate(true);
	}

	userCanAddItemsManually = linkedTemplateManager == nullptr;
}

void CustomValuesCommandArgumentManager::rebuildFromTemplate(bool clearItems)
{
	if (linkedTemplateManager == nullptr || linkedTemplateManagerRef.wasObjectDeleted()) return;

	if (clearItems) clear();

	for (auto& i : linkedTemplateManager->items)
	{
		if (i == nullptr) continue;

		CustomValuesCommandArgument* a = nullptr;
		if (clearItems)
		{
			a = addItemFromData(i->getJSONData());
			a->setNiceName(i->niceName);
		}
		else a = items[linkedTemplateManager->items.indexOf(i)];

		jassert(a != nullptr);

		if (a != nullptr)
		{
			a->linkToTemplate(i);
			a->userCanRemove = false;
			a->userCanDuplicate = false;
		}
	}

	hideInEditor = items.size() == 0;

}

void CustomValuesCommandArgumentManager::addItemInternal(CustomValuesCommandArgument* item, var data)
{
	autoRenameItems();

	if (mappingEnabled)
	{
		if (!isCurrentlyLoadingData)
		{
			if (linkedTemplateManager == nullptr && mappingEnabled && items.size() == 1)
			{
				if (item->paramLink != nullptr)
				{
					item->paramLink->mappingValueIndex = 0;
					item->paramLink->setLinkType(ParameterLink::MAPPING_INPUT);
				}
			}
		}

		item->paramLink->inputValueNames = inputNames;
	}
}

void CustomValuesCommandArgumentManager::addItemsInternal(Array<CustomValuesCommandArgument*> items, var data)
{
	autoRenameItems();

	if (mappingEnabled)
	{
		if (!isCurrentlyLoadingData)
		{
			for (auto& item : items)
			{
				if (linkedTemplateManager == nullptr && mappingEnabled && items.size() == 1)
				{
					if (item->paramLink != nullptr)
					{
						item->paramLink->mappingValueIndex = 0;
						item->paramLink->setLinkType(ParameterLink::MAPPING_INPUT);
					}
				}

				item->paramLink->inputValueNames = inputNames;
			}
		}
	}
}

void CustomValuesCommandArgumentManager::removeItemInternal(CustomValuesCommandArgument* i)
{
	autoRenameItems();
}

void CustomValuesCommandArgumentManager::removeItemsInternal(Array<CustomValuesCommandArgument*> items)
{
	autoRenameItems();
}


CustomValuesCommandArgument* CustomValuesCommandArgumentManager::createItemWithParam(Parameter* p)
{
	CustomValuesCommandArgument* a = new CustomValuesCommandArgument("#" + String(items.size() + 1), p, mappingEnabled, templateMode, multiplex, enablePrecison);
	return a;
}

CustomValuesCommandArgument* CustomValuesCommandArgumentManager::createItemFromType(Parameter::Type type)
{
	Parameter* p = createParameterFromType(type);
	if (p == nullptr) return nullptr;

	return createItemWithParam(p);
}

Parameter* CustomValuesCommandArgumentManager::createParameterFromType(Parameter::Type type)
{
	if (!linkedTemplateManagerRef.wasObjectDeleted() && linkedTemplateManager != nullptr) return linkedTemplateManager->createParameterFromType(type);

	String id = String(items.size() + 1);

	Parameter* p = nullptr;

	switch (type)
	{
	case Parameter::STRING:
		p = new StringParameter("#" + id, "Argument #" + id + ", type int", "example");
		break;
	case Parameter::FLOAT:
		p = new FloatParameter("#" + id, "Argument #" + id + ", type float", 0);
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

	if (p != nullptr) p->isCustomizableByUser = true;

	if (createParamCallbackFunc != nullptr) createParamCallbackFunc(p);

	return p;
}

CustomValuesCommandArgument* CustomValuesCommandArgumentManager::addItemFromData(var data, bool addToUndo)
{

	Controllable::Type t = (Controllable::Type)Controllable::typeNames.indexOf(data.getProperty("type", ""));
	CustomValuesCommandArgument* item = createItemFromType(t);
	return addItem(item);

	/*if (s.isEmpty()) return nullptr;

	Parameter * p =  (Parameter *)ControllableFactory::createControllable(s);
	if (p == nullptr)
	{
		LOG("Error loading custom argument !");
		return nullptr;
	}

	return addItemWithParam(p, data, fromUndoableAction);
	*/
}

Array<CustomValuesCommandArgument*> CustomValuesCommandArgumentManager::addItemsFromData(var data, bool addToUndo)
{
	Array<CustomValuesCommandArgument*> itemsToAdd;
	for (int i = 0; i < data.size(); i++)
	{
		Controllable::Type t = (Controllable::Type)Controllable::typeNames.indexOf(data[i].getProperty("type", ""));
		CustomValuesCommandArgument* item = createItemFromType(t);
		itemsToAdd.add(item);
	}

	return addItems(itemsToAdd, data, addToUndo);
}

var CustomValuesCommandArgumentManager::addItemWithTypeFromScript(const var::NativeFunctionArgs& a)
{

	CustomValuesCommandArgumentManager* m = getObjectFromJS<CustomValuesCommandArgumentManager>(a);

	if (m == nullptr) return var();
	if (!checkNumArgs("Arguments", a, 1)) return var();

	CustomValuesCommandArgument* arg = m->createItemFromType((Parameter::Type)Parameter::typeNames.indexOf(a.arguments[0].toString()));
	if (arg != nullptr)
	{
		m->addItem(arg);
		return arg->getScriptObject();
	}

	return var();
}

void CustomValuesCommandArgumentManager::autoRenameItems()
{
	for (int i = 0; i < items.size(); ++i)
	{
		if (items[i]->niceName.startsWithChar('#')) items[i]->setNiceName("#" + String(i + 1));
	}
}

void CustomValuesCommandArgumentManager::setInputNames(StringArray _inputNames)
{
	if (!mappingEnabled) return;
	inputNames = _inputNames;
	for (auto& i : items) i->paramLink->inputValueNames = inputNames;
}

void CustomValuesCommandArgumentManager::itemAdded(CustomValuesCommandArgument* i)
{
	CustomValuesCommandArgument* a = addItemFromData(i->getJSONData());
	a->linkToTemplate(i);
}

void CustomValuesCommandArgumentManager::itemsAdded(Array<CustomValuesCommandArgument*> items)
{
	for (auto& i : items)
	{
		CustomValuesCommandArgument* a = addItemFromData(i->getJSONData());
		a->linkToTemplate(i);
	}
}

void CustomValuesCommandArgumentManager::itemRemoved(CustomValuesCommandArgument* i)
{
	CustomValuesCommandArgument* itemToRemove = nullptr;
	for (auto& it : items)
	{
		if (it->linkedTemplate == i)
		{
			itemToRemove = it;
			break;
		}
	}
	if (itemToRemove != nullptr)
	{
		if (linkedTemplateManager != nullptr && linkedTemplateManager->isBeingDestroyed) itemToRemove->linkToTemplate(nullptr); //do not sync on template destroy so we can keep ghost data
		else removeItem(itemToRemove, false);
	}
}

void CustomValuesCommandArgumentManager::itemsRemoved(Array<CustomValuesCommandArgument*> items)
{
	for (auto& i : items)
	{
		CustomValuesCommandArgument* itemToRemove = nullptr;
		for (auto& it : items)
		{
			if (it->linkedTemplate == i)
			{
				itemToRemove = it;
				break;
			}
		}
		if (itemToRemove != nullptr)
		{
			if (linkedTemplateManager != nullptr && linkedTemplateManager->isBeingDestroyed) itemToRemove->linkToTemplate(nullptr); //do not sync on template destroy so we can keep ghost data
			else removeItem(itemToRemove, false);
		}
	}
}

void CustomValuesCommandArgumentManager::loadJSONDataInternal(var data)
{
	if (linkedTemplateManager == nullptr || !data.isVoid()) BaseManager::loadJSONDataInternal(data);
	rebuildFromTemplate(false); //cannot do without clearing, already cleared by parent method
}

InspectableEditor* CustomValuesCommandArgumentManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CustomValuesCommandArgumentManagerEditor(this, isRoot);
}
