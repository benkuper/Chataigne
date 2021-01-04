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
#include "Action/Condition/conditions/ActivationCondition/ActivationCondition.h"
#include "Multiplex/Multiplex.h"

ProcessorManager::ProcessorManager(const String &name, Multiplex * multiplex) :
	BaseManager<Processor>(name),
	forceDisabled(false)
{
	isSelectable = false;
	itemDataType = "Processor"; 

	managerFactory = &factory;
	factory.defs.add(MultiplexTargetDefinition<Processor>::createDef<Action>("", "Action", multiplex));
	factory.defs.add(MultiplexTargetDefinition<Processor>::createDef<Mapping>("", "Mapping", multiplex));
	if(multiplex == nullptr) factory.defs.add(Factory<Processor>::Definition::createDef<Multiplex>("", "Multiplex"));
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
//	DBG("Check all activate actions");
	Array<Action*> actions = getAllActions();
	for (auto &a : actions)
	{
		for (auto &c : a->cdm.items)
		{
			ActivationCondition * ac = dynamic_cast<ActivationCondition *>(c);
			if (ac != nullptr)
			{
				bool valid = ac->type == ActivationCondition::Type::ON_ACTIVATE && !ac->forceDisabled;
				for(int i = 0;i<ac->getMultiplexCount();i++) ac->setValid(i, valid, false);
			}
		}

		/*
		if (a->actionRoles.contains(Action::ACTIVATE))
		{
			a->cdm.validationProgress->setValue(0);
			a->cdm.checkAllConditions(true, false);
		}
		*/
	}
}

void ProcessorManager::checkAllDeactivateActions()
{
	Array<Action*> actions = getAllActions();
	for (auto &a : actions)
	{
		for (auto &c : a->cdm.items)
		{
			ActivationCondition * ac = dynamic_cast<ActivationCondition *>(c);
			if (ac != nullptr)
			{
				for (int i = 0; i < ac->getMultiplexCount(); i++) ac->setValid(i, ac->type == ActivationCondition::Type::ON_DEACTIVATE);
			}
		}

		/*
		if (a->actionRoles.contains(Action::DEACTIVATE))
		{
			a->cdm.validationProgress->setValue(0);
			a->cdm.checkAllConditions(true);
		}
		*/
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

