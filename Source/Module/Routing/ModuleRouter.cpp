/*
  ==============================================================================

	ModuleRouter.cpp
	Created: 29 Apr 2017 10:35:18am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"

ModuleRouter::ModuleRouter() :
	BaseItem("Router"),
	sourceModule(nullptr),
	destModule(nullptr),
	sourceValues("Source Values"),
	routerController(nullptr)
{
	sourceValues.userCanAddItemsManually = false;
	selectAllValues = addTrigger("Select All", "Select all values for routing");
	deselectAllValues = addTrigger("Deselect All", "Deselect all values");
	routeAllValues = addTrigger("Route All", "Immediately trigger all enabled routes");

	addChildControllableContainer(&sourceValues);
}

ModuleRouter::~ModuleRouter()
{
	setSourceModule(nullptr);
	setDestModule(nullptr);
}

void ModuleRouter::setSourceModule(Module * m)
{
	if (m == sourceModule) return;

	//make sure that destModule is in a null or stable state but not in a fucked state before reassigning all the routerValues
	if (destModule != nullptr && destModuleRef.wasObjectDeleted()) setDestModule(nullptr);

	if (sourceModule != nullptr)
	{
		sourceModule->valuesCC.removeAsyncContainerListener(this);
		sourceModule->removeInspectableListener(this);
		sourceModule->removeControllableContainerListener(this);
		sourceModuleRef = nullptr;
		sourceValues.clear();
	}
	
	sourceModule = m;

	if (sourceModule != nullptr)
	{
		sourceModuleRef = sourceModule;

		sourceModule->valuesCC.addAsyncContainerListener(this);
		sourceModule->addInspectableListener(this);
		sourceModule->addControllableContainerListener(this);

		Array<WeakReference<Controllable>> values = sourceModule->valuesCC.getAllControllables(true);
		int index = 0;
		for (auto &c : values)
		{
			ModuleRouterValue * mrv = new ModuleRouterValue(c, index++);
			sourceValues.addItem(mrv, var(), false);
			mrv->forceDisabled = !enabled->boolValue();
			mrv->setSourceAndOutModule(sourceModule, destModule);
		}
	}

	routerListeners.call(&RouterListener::sourceModuleChanged, this);
}

void ModuleRouter::setDestModule(Module * m)
{
	if (m == destModule) return;

	if (!destModuleRef.wasObjectDeleted() && destModule != nullptr)
	{
		destModule->removeInspectableListener(this);
		destModuleRef = nullptr;
	}

	destModule = m;
	if (destModule != nullptr)
	{
		destModuleRef = destModule;
		destModule->addInspectableListener(this);
	}

	if (!sourceModuleRef.wasObjectDeleted())
	{
		for (auto &mrv : sourceValues.items)
		{
			mrv->setSourceAndOutModule(sourceModule, destModule);
			mrv->forceDisabled = !enabled->boolValue();
		}
	}

	if (routerController != nullptr) removeChildControllableContainer(routerController);
	routerController = destModule != nullptr ? destModule->createModuleRouterController(this) : nullptr;
	if (routerController != nullptr) addChildControllableContainer(routerController, true);

	routerListeners.call(&RouterListener::destModuleChanged, this);
}

void ModuleRouter::reloadSourceValues(bool keepData)
{
	var prevData = sourceValues.getJSONData();
	sourceValues.clear();

	if (sourceModuleRef.wasObjectDeleted() || destModuleRef.wasObjectDeleted()) return;

	Array<WeakReference<Controllable>> values = sourceModule->valuesCC.getAllControllables(true);
	int index = 0;
	for (auto& c : values)
	{
		if (c == nullptr || c.wasObjectDeleted()) continue;
		ModuleRouterValue* mrv = new ModuleRouterValue(c, index++);
		sourceValues.addItem(mrv, var(), false);
		mrv->forceDisabled = !enabled->boolValue();
		mrv->setSourceAndOutModule(sourceModule, destModule);
	}

	if (keepData) sourceValues.loadItemsData(prevData);
}


var ModuleRouter::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (sourceModule != nullptr)
	{
		data.getDynamicObject()->setProperty("sourceModule", sourceModule->shortName);
		data.getDynamicObject()->setProperty("sourceValues", sourceValues.getJSONData());
	}
	if(destModule != nullptr) data.getDynamicObject()->setProperty("destModule", destModule->shortName);

	return data;
}

void ModuleRouter::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);

	String moduleName = data.getProperty("sourceModule", "");
	if (moduleName == CVGroupManager::getInstance()->module->shortName) setSourceModule(CVGroupManager::getInstance()->module.get());
	else setSourceModule(ModuleManager::getInstance()->getItemWithName(moduleName));
	
	setDestModule(ModuleManager::getInstance()->getItemWithName(data.getProperty("destModule", "")));
	
	if (data.getDynamicObject()->hasProperty("sourceValues")) sourceValues.loadItemsData(data.getProperty("sourceValues", var()));
}

void ModuleRouter::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == e.ChildStructureChanged)
	{
		if (!Engine::mainEngine->isLoadingFile && !isCurrentlyLoadingData && !Engine::mainEngine->isClearing)
		{
			reloadSourceValues();
		}
	}
}

ModuleRouterValue * ModuleRouter::getRouterValueForControllable(Controllable * c)
{
	for (auto &v : sourceValues.items)
	{
		if (v->sourceValue == c) return v;
	}

	return nullptr;
}

void ModuleRouter::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled)
	{
		for (auto &mrv : sourceValues.items) mrv->forceDisabled = !enabled->boolValue();
	}
}

void ModuleRouter::onContainerTriggerTriggered(Trigger * t)
{
	if (t == selectAllValues || t == deselectAllValues)
	{
		for (auto &sv : sourceValues.items)
		{
			sv->enabled->setValue(t == selectAllValues);
		}
	}
	else if (t == routeAllValues)
	{
		for (auto& v : sourceValues.items)
		{
			if (v->enabled->value && v->outModule != nullptr)
			{
				v->outModule->handleRoutedModuleValue(v->sourceValue, v->routeParams.get());
			}
		}
	}
}

void ModuleRouter::inspectableDestroyed(Inspectable * i)
{
	if (i == sourceModule) setSourceModule(nullptr);
	else if (i == destModule) setDestModule(nullptr);
}

