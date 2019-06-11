/*
  ==============================================================================

	CVPreset.h
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Morpher/MorphTarget.h"

class CVGroup;

class GenericControllableManagerLinkedContainer :
	public ControllableContainer,
	public GenericControllableManager::Listener
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

	void parameterValueChanged(Parameter*) override;
	void parameterRangeChanged(Parameter*) override;
	void controllableNameChanged(Controllable*) override;

	Parameter* getParameterForSource(Parameter* p);

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

class CVPreset :
	public MorphTarget
{
public:
	CVPreset(CVGroup* group = nullptr);
	~CVPreset();

	CVGroup* group;
	GenericControllableManagerLinkedContainer values;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor* getEditor(bool isRoot) override;
};