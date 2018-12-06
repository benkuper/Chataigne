/*
  ==============================================================================

    ProcessorManager.cpp
    Created: 15 Oct 2017 1:24:52pm
    Author:  Ben

  ==============================================================================
*/

#include "ProcessorManager.h"


#include "Action/Action.h"
#include "Mapping/Mapping.h"

ProcessorManager::ProcessorManager(const String &name) :
	BaseManager<Processor>(name),
	forceDisabled(false)
{
	itemDataType = "Processor"; 
	managerFactory = &factory;
	factory.defs.add(Factory<Processor>::Definition::createDef("", "Action", &Action::create));
	factory.defs.add(Factory<Processor>::Definition::createDef("", "Mapping", &Mapping::create));

}

ProcessorManager::~ProcessorManager()
{
}

void ProcessorManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (auto &i : items) i->setForceDisabled(forceDisabled);
}

void ProcessorManager::addItemInternal(Processor * item, var data)
{
	item->setForceDisabled(forceDisabled);
}

bool ProcessorManager::canAddItemOfType(const String & typeToCheck)
{
	return typeToCheck == itemDataType || typeToCheck == "Action" || typeToCheck == "Mapping";
}

Array<Action*> ProcessorManager::getAllActions()
{
	Array<Action*> result;
	for (auto & i : items)
	{
		if (i->type == Processor::ACTION) result.add(static_cast<Action *>(i));
	}
	return result;
}

Array<Mapping*> ProcessorManager::getAllMappings()
{
	Array<Mapping *> result;
	for (auto & i : items)
	{
		if (i->type == Processor::MAPPING) result.add(static_cast<Mapping *>(i));
	}
	return result;
}

void ProcessorManager::checkAllActivateActions()
{
	Array<Action*> actions = getAllActions();
	for (auto &a : actions)
	{
		if (a->actionRoles.contains(Action::ACTIVATE))
		{
			a->cdm.validationProgress->setValue(0);
			a->cdm.checkAllConditions(true);
		}
	}
}

void ProcessorManager::checkAllDeactivateActions()
{
	Array<Action*> actions = getAllActions();
	for (auto &a : actions)
	{
		if (a->actionRoles.contains(Action::DEACTIVATE))
		{
			a->cdm.validationProgress->setValue(0);
			a->cdm.checkAllConditions(true);
		}
	}
}

void ProcessorManager::processAllMappings()
{
	Array<Mapping*> mappings = getAllMappings();
	for (auto &m : mappings)
		m->process();
}

void ProcessorManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	setForceDisabled(forceDisabled, true);
}

