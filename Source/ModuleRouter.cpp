/*
  ==============================================================================

	ModuleRouter.cpp
	Created: 29 Apr 2017 10:35:18am
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouter.h"
#include "ModuleManager.h"

ModuleRouter::ModuleRouter() :
	BaseItem("Router"),
	sourceModule(nullptr),
	destModule(nullptr),
	sourceValues("Source Values")
{
    
}

ModuleRouter::~ModuleRouter()
{
	setSourceModule(nullptr);
	setDestModule(nullptr);
}

void ModuleRouter::setSourceModule(Module * m)
{
	if (m == sourceModule) return;
	if (sourceModule != nullptr)
	{
		sourceModule->removeInspectableListener(this);
		DBG("clear here !");
		sourceValues.clear();
	}
	
	sourceModule = m;

	if (sourceModule != nullptr)
	{
		sourceModule->addInspectableListener(this);

		Array<WeakReference<Controllable>> values = sourceModule->valuesCC.getAllControllables();
		int index = 0;
		for (auto &c : values)
		{
			ModuleRouterValue * mrv = new ModuleRouterValue(c, index++);
			sourceValues.addItem(mrv);
			mrv->setOutModule(destModule);
		}
	}

	routerListeners.call(&RouterListener::sourceModuleChanged, this);
}

void ModuleRouter::setDestModule(Module * m)
{
	if (m == destModule) return;
	if (destModule != nullptr)
	{
		destModule->removeInspectableListener(this);
	}

	destModule = m;

	if (destModule != nullptr)
	{
		destModule->addInspectableListener(this);
	}

	for (auto &mrv : sourceValues.items)
	{
		mrv->setOutModule(destModule);
	}

	routerListeners.call(&RouterListener::destModuleChanged, this);
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
	setSourceModule(ModuleManager::getInstance()->getItemWithName(data.getProperty("sourceModule", "")));
	setDestModule(ModuleManager::getInstance()->getItemWithName(data.getProperty("destModule", "")));
	if (data.getDynamicObject()->hasProperty("sourceValues")) sourceValues.loadItemsData(data.getProperty("sourceValues", var()));
}

void ModuleRouter::inspectableDestroyed(Inspectable * i)
{
	if (i == sourceModule) setSourceModule(nullptr);
	else if (i == destModule) setDestModule(nullptr);
}
