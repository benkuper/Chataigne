/*
  ==============================================================================

	CustomVariablesModule.cpp
	Created: 22 Feb 2018 6:28:07pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

CustomVariablesModule::CustomVariablesModule(CVGroupManager* manager) :
	Module("Custom Variables"),
	manager(manager)
{
	manager->addBaseManagerListener(this);

	defManager->add(CommandDefinition::createDef(this, "", "Set Value", &GenericControllableCommand::create, CommandContext::BOTH)
		->addParam("action", GenericControllableCommand::SET_VALUE)
		->addParam("root", (int64)(void*)&valuesCC)
		->addParam("labelLevel", 0)
	);

	defManager->add(CommandDefinition::createDef(this, "", "Go to Value", &GenericControllableCommand::create, CommandContext::BOTH)
		->addParam("action", GenericControllableCommand::GO_TO_VALUE)
		->addParam("root", (int64)(void*)&valuesCC)
		->addParam("labelLevel", 0)
	);

	defManager->add(CommandDefinition::createDef(this, "", "Set Preset", &CVCommand::create)->addParam("type", CVCommand::SET_PRESET));
	defManager->add(CommandDefinition::createDef(this, "", "Go to preset", &CVCommand::create)->addParam("type", CVCommand::GO_TO_PRESET));
	defManager->add(CommandDefinition::createDef(this, "", "Kill Go to preset", &CVCommand::create, CommandContext::ACTION)->addParam("type", CVCommand::KILL_GO_TO_PRESET));
	defManager->add(CommandDefinition::createDef(this, "", "Interpolate presets", &CVCommand::create)->addParam("type", CVCommand::LERP_PRESETS));
	defManager->add(CommandDefinition::createDef(this, "", "Set Preset Weight", &CVCommand::create)->addParam("type", CVCommand::SET_PRESET_WEIGHT));
	defManager->add(CommandDefinition::createDef(this, "", "Set Morpher Target", &CVCommand::create)->addParam("type", CVCommand::SET_2DTARGET));
	defManager->add(CommandDefinition::createDef(this, "", "Randomize", &CVCommand::create)->addParam("type", CVCommand::RANDOMIZE));
	defManager->add(CommandDefinition::createDef(this, "", "Load Preset from file", &CVCommand::create)->addParam("type", CVCommand::LOAD_PRESET));
	defManager->add(CommandDefinition::createDef(this, "", "Save Preset to file", &CVCommand::create)->addParam("type", CVCommand::SAVE_PRESET));
}

CustomVariablesModule::~CustomVariablesModule()
{
	manager->removeBaseManagerListener(this);
	clearItems();
}


GenericControllableManagerLinkedContainer* CustomVariablesModule::getValueCCForGroup(CVGroup* g)
{
	for (auto& cc : valuesContainers) if (cc->manager == &g->values) return cc;
	return nullptr;
}

void CustomVariablesModule::clearItems()
{
	while (valuesContainers.size() > 0)
	{
		valuesCC.removeChildControllableContainer(valuesContainers[0]);

		CVGroup* g = dynamic_cast<CVGroup*>(valuesContainers[0]->manager->parentContainer.get());
		jassert(g != nullptr);
		g->removeControllableContainerListener(this);

		valuesContainers.removeObject(valuesContainers[0]);
	}

}


void CustomVariablesModule::itemAdded(CVGroup* g)
{
	GenericControllableManagerLinkedContainer* cc = new GenericControllableManagerLinkedContainer(g->niceName, &g->values, true);
	valuesCC.addChildControllableContainer(cc);
	g->addControllableContainerListener(this);
	valuesContainers.add(cc);
}

void CustomVariablesModule::itemsAdded(Array<CVGroup*> groups)
{
	for (auto& g : groups)
	{
		GenericControllableManagerLinkedContainer* cc = new GenericControllableManagerLinkedContainer(g->niceName, &g->values, true);
		valuesCC.addChildControllableContainer(cc);
		g->addControllableContainerListener(this);
		valuesContainers.add(cc);
	}
}

void CustomVariablesModule::itemRemoved(CVGroup* g)
{
	GenericControllableManagerLinkedContainer* cc = getValueCCForGroup(g);
	if (cc == nullptr) return;

	valuesCC.removeChildControllableContainer(cc);
	g->removeControllableContainerListener(this);
	valuesContainers.removeObject(cc);
}

void CustomVariablesModule::itemsRemoved(Array<CVGroup*> groups)
{
	for (auto& g : groups)
	{
		GenericControllableManagerLinkedContainer* cc = getValueCCForGroup(g);
		if (cc == nullptr) return;

		valuesCC.removeChildControllableContainer(cc);
		g->removeControllableContainerListener(this);
		valuesContainers.removeObject(cc);
	}
}

void CustomVariablesModule::childAddressChanged(ControllableContainer* cc)
{
	Module::childAddressChanged(cc);

	CVGroup* g = dynamic_cast<CVGroup*>(cc);
	if (g == nullptr) return;
	GenericControllableManagerLinkedContainer* mlc = getValueCCForGroup(g);
	if (mlc == nullptr) return;

	mlc->setNiceName(g->niceName);
}



//Linked Container

GenericControllableManagerLinkedContainer::GenericControllableManagerLinkedContainer(const String& name, GenericControllableManager* manager, bool keepValuesInSync) :
	ControllableContainer(name),
	manager(manager),
	keepValuesInSync(keepValuesInSync),
	linkedComparator(manager)
{
	manager->addBaseManagerListener(this);
	resetAndBuildValues(keepValuesInSync);
}

GenericControllableManagerLinkedContainer::~GenericControllableManagerLinkedContainer()
{
	manager->removeBaseManagerListener(this);

	HashMap<Parameter*, Parameter*>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

}

void GenericControllableManagerLinkedContainer::resetAndBuildValues(bool syncValues)
{
	HashMap<Parameter*, Parameter*>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

	clear();
	for (auto& gci : manager->items)
	{
		addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
	}
}

void GenericControllableManagerLinkedContainer::addValueFromItem(Parameter* source)
{
	if (source == nullptr) return;

	Controllable* c = ControllableFactory::createControllable(source->getTypeString());
	Parameter* p = dynamic_cast<Parameter*>(c);


	linkMap.set(p, source);
	source->addControllableListener(this);
	source->addParameterListener(this);
	if (source->type == source->ENUM) ((EnumParameter*)source)->addEnumParameterListener(this);
	p->forceSaveValue = true;
	syncItem(p, source);
	addParameter(p);
}

void GenericControllableManagerLinkedContainer::syncItem(Parameter* p, Parameter* source, bool syncValue)
{
	p->setNiceName(source->niceName);

	if (source->hasRange()) p->setRange(source->minimumValue, source->maximumValue);
	else p->clearRange();

	p->alwaysNotify = source->alwaysNotify;
	p->setControllableFeedbackOnly(source->isControllableFeedbackOnly);

	if (p->type == Parameter::ENUM)
	{
		EnumParameter* es = (EnumParameter*)source;
		EnumParameter* ep = (EnumParameter*)p;
		if (es->enumValues.size() != ep->enumValues.size())
		{
			ep->clearOptions();
			String key = ep->getValueKey();
			for (auto& ev : es->enumValues) ep->addOption(ev->key, ev->value, false);
			ep->setValueWithKey(key);
		}
	}

	if (syncValue)
	{
		if (p->type != Parameter::ENUM) p->setValue(source->value);
		else ((EnumParameter*)p)->setValueWithKey(((EnumParameter*)source)->getValueKey());
	}
}

void GenericControllableManagerLinkedContainer::syncItems(bool syncValues)
{
	Array<WeakReference<Parameter>> pList = getAllParameters();
	for (auto& p : pList)
	{
		syncItem(p, linkMap[p], syncValues);
	}
}

void GenericControllableManagerLinkedContainer::itemAdded(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
}

void GenericControllableManagerLinkedContainer::itemRemoved(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	Parameter* p = dynamic_cast<Parameter*>(getControllableByName(gci->niceName, true));
	if (p != nullptr)
	{
		linkMap[p]->removeControllableListener(this);
		linkMap[p]->removeParameterListener(this);
		if (linkMap[p]->type == linkMap[p]->ENUM) ((EnumParameter*)linkMap[p])->removeEnumParameterListener(this);
		linkMap.remove(p);
		removeControllable(p);
	}
}

void GenericControllableManagerLinkedContainer::itemsReordered()
{
	controllables.sort(linkedComparator);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerReordered, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerReordered, this));
}

void GenericControllableManagerLinkedContainer::enumOptionAdded(EnumParameter* source, const String&)
{
	Parameter* p = getParameterForSource(source);
	syncItem(p, source);
}

void GenericControllableManagerLinkedContainer::enumOptionUpdated(EnumParameter* source, int index, const String&, const String&)
{
	Parameter* p = getParameterForSource(source);
	syncItem(p, source);
}

void GenericControllableManagerLinkedContainer::enumOptionRemoved(EnumParameter* source, const String&)
{
	Parameter* p = getParameterForSource(source);
	syncItem(p, source);
}


void GenericControllableManagerLinkedContainer::onContainerParameterChanged(Parameter* p)
{
	ControllableContainer::onContainerParameterChanged(p);
	if (!keepValuesInSync) return;
	if (changingParams.contains(p)) return;

	if (Parameter* tp = getSourceForParameter(p))
	{
		changingParams.add(tp);
		tp->setValue(p->value);
		changingParams.removeAllInstancesOf(tp);
	}
}

void GenericControllableManagerLinkedContainer::onExternalParameterValueChanged(Parameter* p)
{
	ControllableContainer::onExternalParameterValueChanged(p);
	if (!keepValuesInSync) return;
	if (changingParams.contains(p)) return;

	if (Parameter* tp = getParameterForSource(p))
	{
		changingParams.add(tp);
		if (p->type == Parameter::ENUM) ((EnumParameter*)tp)->setValueWithKey(((EnumParameter*)p)->getValueKey());
		else tp->setValue(p->getValue());
		changingParams.removeAllInstancesOf(tp);
	}
}


void GenericControllableManagerLinkedContainer::parameterRangeChanged(Parameter* source)
{
	ControllableContainer::parameterRangeChanged(source);
	Parameter* p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, keepValuesInSync);
}

void GenericControllableManagerLinkedContainer::controllableNameChanged(Controllable* sourceC)
{
	Parameter* source = dynamic_cast<Parameter*>(sourceC);
	if (source == nullptr) return;

	Parameter* p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, keepValuesInSync);
}

void GenericControllableManagerLinkedContainer::controllableAttributeChanged(Controllable* c, const String&)
{
	Parameter* source = dynamic_cast<Parameter*>(c);
	if (source == nullptr) return;

	Parameter* p = getParameterForSource(source);
	if (p == nullptr) return;

	syncItem(p, source, false);
}


Parameter* GenericControllableManagerLinkedContainer::getParameterForSource(Parameter* p)
{
	HashMap<Parameter*, Parameter*>::Iterator i(linkMap);
	while (i.next()) if (p == i.getValue()) return i.getKey();
	return nullptr;
}

Parameter* GenericControllableManagerLinkedContainer::getSourceForParameter(Parameter* p)
{
	return linkMap.contains(p) ? linkMap[p] : nullptr;
}

