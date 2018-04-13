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
	factory.defs.add(Factory<Processor>::Definition::createDef("", "Action", &Action::create)->addParam("hasOffConsequences",true));
	factory.defs.add(Factory<Processor>::Definition::createDef("", "Mapping", &Mapping::create));
	factory.defs.add(Factory<Processor>::Definition::createDef("", "On Activate", &Action::create)->addParam("role", Action::ACTIVATE)->addParam("name","On Activate"));
	factory.defs.add(Factory<Processor>::Definition::createDef("", "On Deactivate", &Action::create)->addParam("role", Action::DEACTIVATE)->addParam("name","On Deactivate"));

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

void ProcessorManager::triggerAllActivateActions()
{
	Array<Action*> actions = getAllActions();
	for (auto &a : actions)
	{
		if (a->actionRole == Action::ACTIVATE)
		{
			a->triggerOn->trigger();
		}
	}
}

void ProcessorManager::triggerAllDeactivateActions()
{
	Array<Action*> actions = getAllActions();
	for (auto &a : actions) if (a->actionRole == Action::DEACTIVATE) a->triggerOn->trigger();
}

void ProcessorManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	setForceDisabled(forceDisabled, true);
}

