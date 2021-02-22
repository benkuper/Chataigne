/*
  ==============================================================================

    CustomVariablesModule.h
    Created: 22 Feb 2018 6:28:07pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "CustomVariables/CVGroupManager.h"
#include "../../Module.h"

class GenericControllableManagerLinkedContainer;

class CustomVariablesModule :
	public Module,
	public CVGroupManager::ManagerListener
{
public:
	CustomVariablesModule(CVGroupManager * manager);
	~CustomVariablesModule();

	CVGroupManager * manager;
	OwnedArray<GenericControllableManagerLinkedContainer> valuesContainers;

	GenericControllableManagerLinkedContainer* getValueCCForGroup(CVGroup * c);

	void clearItems();

	void itemAdded(CVGroup* g) override;
	void itemsAdded(Array<CVGroup*> g) override;
	void itemRemoved(CVGroup* g) override;
	void itemsRemoved(Array<CVGroup *> g) override;
	void childAddressChanged(ControllableContainer * cc) override;

	String getDefaultTypeString() const override { return "CustomVariables"; }
};


class GenericControllableManagerLinkedContainer :
	public ControllableContainer,
	public GenericControllableManager::ManagerListener,
	public EnumParameter::Listener
{
public:
	GenericControllableManagerLinkedContainer(const String& name, GenericControllableManager* manager, bool keepValuesInSync);
	~GenericControllableManagerLinkedContainer();

	GenericControllableManager* manager;
	HashMap<Parameter*, Parameter*> linkMap;

	bool keepValuesInSync;

	void resetAndBuildValues(bool syncValues = true);

	void addValueFromItem(Parameter* source);
	void syncItem(Parameter* p, Parameter* source, bool syncValue = true);
	void syncItems(bool syncValues);

	void itemAdded(GenericControllableItem*) override;
	void itemRemoved(GenericControllableItem*) override;
	void itemsReordered() override;
	void enumOptionAdded(EnumParameter* source, const String&) override;
	void enumOptionUpdated(EnumParameter* source, int index, const String&, const String&) override;
	void enumOptionRemoved(EnumParameter * source, const String &) override;

	void onContainerParameterChanged(Parameter* p) override;
	void onExternalParameterValueChanged(Parameter * p) override;
	void parameterRangeChanged(Parameter*) override;
	void controllableNameChanged(Controllable*) override;

	Parameter* getParameterForSource(Parameter* p);
	Parameter* getSourceForParameter(Parameter* p);

	class LinkedComparator
	{
	public:
		LinkedComparator(GenericControllableManager* manager) : manager(manager) {}
		GenericControllableManager* manager;
		int compareElements(Controllable* c1, Controllable* c2)
		{
			int i1 = manager->items.indexOf(manager->getItemWithName(c1->shortName));
			int i2 = manager->items.indexOf(manager->getItemWithName(c2->shortName));
			if (i1 == i2) return 0;
			return i1 > i2 ? 1 : -1;
		}
	};

	LinkedComparator linkedComparator;

};