/*
  ==============================================================================

    CustomVariablesModule.cpp
    Created: 22 Feb 2018 6:28:07pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomVariablesModule.h"
#include "commands/CustomVariablesModuleCommands.h"

CustomVariablesModule::CustomVariablesModule(CVGroupManager * manager) :
	Module("Custom Variables"),
	manager(manager)
{
	manager->addBaseManagerListener(this);

	defManager.add(CommandDefinition::createDef(this, "", "Set Value", &CVCommand::create, CommandContext::BOTH)->addParam("type", CVCommand::SET_VALUE));
	defManager.add(CommandDefinition::createDef(this, "", "Set Preset", &CVCommand::create, CommandContext::ACTION)->addParam("type", CVCommand::SET_PRESET));
	defManager.add(CommandDefinition::createDef(this, "", "Interpolate", &CVCommand::create, CommandContext::BOTH)->addParam("type", CVCommand::LERP_PRESETS));
	defManager.add(CommandDefinition::createDef(this, "", "Set Preset Weight", &CVCommand::create, CommandContext::BOTH)->addParam("type", CVCommand::SET_PRESET_WEIGHT));
	defManager.add(CommandDefinition::createDef(this,"", "Set Target Position", &CVCommand::create, CommandContext::BOTH)->addParam("type", CVCommand::SET_2DTARGET));
}

CustomVariablesModule::~CustomVariablesModule()
{
	manager->removeBaseManagerListener(this);
	clearItems();
}


GenericControllableManagerLinkedContainer * CustomVariablesModule::getValueCCForGroup(CVGroup * g)
{
	for (auto &cc : valuesContainers) if (cc->manager == &g->values) return cc;
	return nullptr;
}

void CustomVariablesModule::clearItems()
{
	while (valuesContainers.size() > 0)
	{
		valuesCC.removeChildControllableContainer(valuesContainers[0]);
		
		CVGroup * g = dynamic_cast<CVGroup *>(valuesContainers[0]->manager->parentContainer.get());
		jassert(g != nullptr);
		g->removeControllableContainerListener(this);

		valuesContainers.removeObject(valuesContainers[0]);
	}

}

void CustomVariablesModule::itemAdded(CVGroup * g)
{
	GenericControllableManagerLinkedContainer * cc = new GenericControllableManagerLinkedContainer(g->niceName,&g->values,true);
	valuesCC.addChildControllableContainer(cc);
	g->addControllableContainerListener(this);
	valuesContainers.add(cc);
}

void CustomVariablesModule::itemRemoved(CVGroup * g)
{
	GenericControllableManagerLinkedContainer * cc = getValueCCForGroup(g);
	if (cc == nullptr) return;
	
	valuesCC.removeChildControllableContainer(cc);
	g->removeControllableContainerListener(this);
	valuesContainers.removeObject(cc);
}

void CustomVariablesModule::childAddressChanged(ControllableContainer * cc)
{
	Module::childAddressChanged(cc);

	CVGroup * g = dynamic_cast<CVGroup *>(cc);
	if (g == nullptr) return;
	GenericControllableManagerLinkedContainer * mlc = getValueCCForGroup(g);
	if (mlc == nullptr) return;
	
	mlc->setNiceName(g->niceName);
}
