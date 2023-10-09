/*
  ==============================================================================

	CVPreset.h
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#pragma once

class CVGroup;

class ParameterPreset :
	public ControllableContainer
{
public:
	ParameterPreset(Parameter * p);
	~ParameterPreset();

	Parameter* parameter;

	enum InterpolationMode { CHANGE_AT_END, CHANGE_AT_START, INTERPOLATE, NONE};
	EnumParameter* interpolationMode;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};

class PresetParameterContainer :
	public ControllableContainer,
	public GenericControllableManager::ManagerListener
{
public:
	PresetParameterContainer(const String& name, GenericControllableManager* manager, bool keepValuesInSync);
	~PresetParameterContainer();

	GenericControllableManager* manager;
	//OwnedArray<ParameterPreset> presets;
	HashMap<ParameterPreset*,  Parameter*> linkMap;

	bool keepValuesInSync;

	void resetAndBuildValues(bool syncValues = true);

	void addValueFromItem(Parameter* source);
	void syncItem(ParameterPreset* preset, bool syncValue = true);
	void syncItems(bool syncValues);

	void syncValues(bool addToUndo = false);
	UndoableAction* syncValue(ParameterPreset* preset, bool onlyReturnUndoAction = false);

	void itemAdded(GenericControllableItem*) override;
	void itemsAdded(Array<GenericControllableItem*> items) override;
	void itemRemoved(GenericControllableItem*) override;
	void itemsRemoved(Array<GenericControllableItem*> items) override;
	void itemsReordered() override;

	void parameterValueChanged(Parameter*) override;
	void parameterRangeChanged(Parameter*) override;
	void controllableNameChanged(Controllable*) override;

	ParameterPreset * getParameterPresetForSource(Parameter* p);

	//var getJSONData() override;
	void loadJSONData(var data, bool createIfNotThere = false) override;

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
	FloatParameter* defaultLoadTime;
	Trigger* loadTrigger;
	Trigger* updateTrigger;

	PresetParameterContainer values;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	var getValuesAsJSON();
	void loadValuesFromJSON(var data);

	void onContainerTriggerTriggered(Trigger* t) override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};