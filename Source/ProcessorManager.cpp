/*
  ==============================================================================

    ProcessorManager.cpp
    Created: 15 Oct 2017 1:24:52pm
    Author:  Ben

  ==============================================================================
*/

#include "ProcessorManager.h"


#include "Action.h"
#include "Mapping.h"

ProcessorManager::ProcessorManager(const String &name) :
	BaseManager<Processor>(name),
	forceDisabled(false)
{
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

void ProcessorManager::loadJSONDataInternal(var data)
{
	setForceDisabled(forceDisabled, true);
}

