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

	GenericControllableManagerLinkedContainer * getValueCCForGroup(CVGroup * c);

	void clearItems();


	void itemAdded(CVGroup * g) override;
	void itemRemoved(CVGroup * g) override;
	void childAddressChanged(ControllableContainer * cc) override;

	String getDefaultTypeString() const override { return "CustomVariables"; }
};