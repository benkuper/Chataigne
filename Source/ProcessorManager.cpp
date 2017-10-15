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

juce_ImplementSingleton(ProcessorFactory)

ProcessorManager::ProcessorManager(const String &name) :
	BaseManager<Processor>(name),
	forceDisabled(false)
{
	managerFactory = ProcessorFactory::getInstance();
}

ProcessorManager::~ProcessorManager()
{
}

void ProcessorManager::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;
	for (auto &i : items) i->forceDisabled = forceDisabled;
}

void ProcessorManager::addItemInternal(Processor * item, var data)
{
	item->forceDisabled = forceDisabled;
}


ProcessorFactory::ProcessorFactory()
{
	defs.add(Definition::createDef("", "Action", &Action::create));
	defs.add(Definition::createDef("", "Mapping", &Mapping::create));
}