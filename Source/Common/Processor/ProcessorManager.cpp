/*
  ==============================================================================

	ProcessorManager.cpp
	Created: 15 Oct 2017 1:24:52pm
	Author:  Ben

  ==============================================================================
*/

ProcessorManager::ProcessorManager(const String& name, Multiplex* multiplex) :
	BaseManager<Processor>(name),
	forceDisabled(false)
{
	isSelectable = false;

	managerFactory = &factory;
	factory.defs.add(MultiplexTargetDefinition<Processor>::createDef<Action>("", "Action", multiplex));
	factory.defs.add(MultiplexTargetDefinition<Processor>::createDef<Mapping>("", "Mapping", multiplex));
	if (multiplex == nullptr)
	{
		factory.defs.add(Factory<Processor>::Definition::createDef<Multiplex>("", "Multiplex"));
		factory.defs.add(Factory<Processor>::Definition::createDef<Conductor>("", "Conductor"));
	}
}

ProcessorManager::~ProcessorManager()
{
}

void ProcessorManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (auto& i : items) i->setForceDisabled(forceDisabled);
}

void ProcessorManager::addItemInternal(Processor* item, var data)
{
	item->setForceDisabled(forceDisabled);
}

void ProcessorManager::addItemsInternal(Array<Processor*> item, var data)
{
	for (auto& item : items) item->setForceDisabled(forceDisabled);
}

bool ProcessorManager::canAddItemOfType(const String& typeToCheck)
{
	return typeToCheck == itemDataType || typeToCheck == "Action" || typeToCheck == "Mapping" || typeToCheck == "Multiplex";
}

Array<Action*> ProcessorManager::getAllActions(bool includeMultiplexes, bool includeConductors)
{
	Array<Action*> result;
	for (auto& i : items)
	{
		if (i == nullptr) continue;
		switch (i->type)
		{
		case Processor::ACTION: result.add(dynamic_cast<Action*>(i)); break;
		case Processor::MULTIPLEX: if (includeMultiplexes) result.addArray(((Multiplex*)i)->processorManager.getAllActions()); break;
		case Processor::CONDUCTOR: if (includeConductors) result.addArray(((Conductor*)i)->processorManager.getAllActions()); break;

            default:
                break;
		}
	}
	return result;
}

Array<Mapping*> ProcessorManager::getAllMappings(bool includeMultiplexes)
{
	Array<Mapping*> result;
	for (auto& i : items)
	{
		if (i->type == Processor::MAPPING) result.add(static_cast<Mapping*>(i));
		else if (includeMultiplexes && i->type == Processor::MULTIPLEX) result.addArray(((Multiplex*)i)->processorManager.getAllMappings());
	}
	return result;
}

void ProcessorManager::checkAllActivateActions()
{
	//	DBG("Check all activate actions");
	Array<Action*> actions = getAllActions();
	for (auto& a : actions)
	{
		if (a->cdm == nullptr) continue;
		for (auto& c : a->cdm->items)
		{
			ActivationCondition* ac = dynamic_cast<ActivationCondition*>(c);
			if (ac != nullptr)
			{
				bool valid = ac->type == ActivationCondition::Type::ON_ACTIVATE && !ac->forceDisabled;
				for (int i = 0; i < ac->getMultiplexCount(); i++) ac->setValid(i, valid, false);
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
	for (auto& a : actions)
	{
		if (a->cdm == nullptr) continue;
		for (auto& c : a->cdm->items)
		{
			ActivationCondition* ac = dynamic_cast<ActivationCondition*>(c);
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

void ProcessorManager::processAllMappings(bool fromLoad)
{
	Array<Mapping*> mappings = getAllMappings();
	for (auto& m : mappings)
	{
		if (fromLoad && !m->sendAfterLoad->boolValue()) continue;
		m->process();
	}
}

void ProcessorManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	setForceDisabled(forceDisabled, true);
}

