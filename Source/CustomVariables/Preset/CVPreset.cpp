/*
  ==============================================================================

	CVPreset.cpp
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"

CVPreset::CVPreset(CVGroup* group) :
	MorphTarget("Preset"),
	group(group),
	values("Values", &group->values, false)
{
	jassert(group != nullptr);

	defaultLoadTime = addFloatParameter("Default Load Time", "The time to use by default when loading this preset", 1, 0);
	defaultLoadTime->defaultUI = FloatParameter::TIME;
	defaultLoadTime->hideInEditor = true;

	loadTrigger = addTrigger("Load", "Load this preset with the default time and the default interpolation curve");
	loadTrigger->hideInEditor = true;

	updateTrigger = addTrigger("Update", "This will update all the preset's values with the current ones from the Group");
	updateTrigger->hideInEditor = true;

	values.hideEditorHeader = true;
	values.editorCanBeCollapsed = false;

	showInspectorOnSelect = false;

	CVGroup::ControlMode cm = group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>();

	weight->setControllableFeedbackOnly(cm == CVGroup::FREE || cm == CVGroup::VORONOI || cm == CVGroup::GRADIENT_BAND);

	addChildControllableContainer(&values);

}

CVPreset::~CVPreset()
{
}

var CVPreset::getJSONData()
{
	var data = MorphTarget::getJSONData();
	data.getDynamicObject()->setProperty(values.shortName, values.getJSONData());
	return data;
}

void CVPreset::loadJSONDataInternal(var data)
{
	MorphTarget::loadJSONDataInternal(data);
	values.loadJSONData(data.getProperty(values.shortName, var()), true);
}

var CVPreset::getValuesAsJSON()
{
	var data = new DynamicObject();
	Array<WeakReference<Parameter>> params = values.getAllParameters();
	for (auto& p : params) data.getDynamicObject()->setProperty(p->shortName, p->value);

	return data;
}

void CVPreset::loadValuesFromJSON(var data)
{
	if (!data.isObject())
	{
		NLOGWARNING(niceName, "Can't load preset values, data is not a json object");
		return;
	}

	NamedValueSet props = data.getDynamicObject()->getProperties();
	for (auto& nv : props)
	{
		Parameter* p = values.getParameterByName(nv.name.toString());
		if (p != nullptr) p->setValue(nv.value);
	}
}

void CVPreset::onContainerTriggerTriggered(Trigger* t)
{
	MorphTarget::onContainerTriggerTriggered(t);
	if (t == loadTrigger) group->goToPreset(this, defaultLoadTime->floatValue(), &group->defaultInterpolation);
	else if (t == updateTrigger) values.syncValues(true);
}

InspectableEditor* CVPreset::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CVPresetEditor(this, isRoot);
}

PresetParameterContainer::PresetParameterContainer(const String& name, GenericControllableManager* manager, bool keepValuesInSync) :
	ControllableContainer(name),
	manager(manager),
	keepValuesInSync(keepValuesInSync),
	linkedComparator(manager)
{
	saveAndLoadRecursiveData = true;

	manager->addBaseManagerListener(this);
	resetAndBuildValues(keepValuesInSync);
}

PresetParameterContainer::~PresetParameterContainer()
{
	manager->removeBaseManagerListener(this);

	HashMap<ParameterPreset*, Parameter*>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}
	linkMap.clear();

}

void PresetParameterContainer::resetAndBuildValues(bool syncValues)
{
	HashMap<ParameterPreset*, Parameter*>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

	clear();
	linkMap.clear();

	for (auto& gci : manager->items)
	{
		addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
	}
}

void PresetParameterContainer::addValueFromItem(Parameter* source)
{
	if (source == nullptr) return;

	Controllable* c = ControllableFactory::createControllable(source->getTypeString());
	Parameter* p = dynamic_cast<Parameter*>(c);
	ParameterPreset* pp = new ParameterPreset(p);
	linkMap.set(pp, source);
	source->addControllableListener(this);
	source->addParameterListener(this);
	p->forceSaveValue = true;
	syncItem(pp);
	addChildControllableContainer(pp, true);
}

void PresetParameterContainer::syncItem(ParameterPreset* preset, bool syncValueAfter)
{
	Parameter* p = preset->parameter;
	Parameter* source = linkMap[preset];

	preset->setNiceName(source->niceName);
	p->setNiceName(source->niceName);

	if (source->hasRange()) p->setRange(source->minimumValue, source->maximumValue);
	else p->clearRange();

	if (p->type == Parameter::ENUM)
	{
		EnumParameter* es = (EnumParameter*)source;
		EnumParameter* ep = (EnumParameter*)p;
		if (es->enumValues.size() != ep->enumValues.size())
		{
			String key = ep->getValueKey();
			for (auto& ev : es->enumValues) ep->addOption(ev->key, ev->value, false);
			ep->setValueWithKey(key);
		}
	}

	if (syncValueAfter) syncValue(preset);

}

void PresetParameterContainer::syncItems(bool syncValues)
{
	for (auto& cc : controllableContainers)
	{
		if (ParameterPreset* pp = dynamic_cast<ParameterPreset*>(cc.get())) syncItem(pp, syncValues);
	}
}

void PresetParameterContainer::syncValues(bool addToUndo)
{
	Array<UndoableAction*> actions;
	for (auto& cc : controllableContainers)
	{
		if (ParameterPreset* pp = dynamic_cast<ParameterPreset*>(cc.get()))
		{
			if (addToUndo) actions.add(syncValue(pp, true));
			else syncValue(pp, false);
		}
	}
	if (addToUndo) UndoMaster::getInstance()->performActions("Update preset " + niceName, actions);
}

UndoableAction* PresetParameterContainer::syncValue(ParameterPreset* preset, bool onlyReturnUndoAction)
{
	Parameter* p = preset->parameter;
	Parameter* source = linkMap[preset];

	if (onlyReturnUndoAction) return p->setUndoableValue(p->value, source->value, true);

	p->setValue(source->value);
	return nullptr;
}

void PresetParameterContainer::itemAdded(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
}

void PresetParameterContainer::itemsAdded(Array<GenericControllableItem*> items)
{
	for (auto& gci : items)
	{
		if (gci->controllable->type == Controllable::TRIGGER) continue;
		addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
	}
}

void PresetParameterContainer::itemRemoved(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	ParameterPreset* pp = dynamic_cast<ParameterPreset*>(getControllableContainerByName(gci->niceName, true));
	if (pp != nullptr)
	{
		linkMap[pp]->removeControllableListener(this);
		linkMap[pp]->removeParameterListener(this);
		linkMap.remove(pp);

		removeChildControllableContainer(pp);
	}
}

void PresetParameterContainer::itemsRemoved(Array<GenericControllableItem*> items)
{
	for (auto& gci : items)
	{
		if (gci->controllable->type == Controllable::TRIGGER) continue;
		ParameterPreset* pp = dynamic_cast<ParameterPreset*>(getControllableContainerByName(gci->niceName, true));
		if (pp != nullptr)
		{
			linkMap[pp]->removeControllableListener(this);
			linkMap[pp]->removeParameterListener(this);
			linkMap.remove(pp);

			removeChildControllableContainer(pp);
		}
	}
}


void PresetParameterContainer::itemsReordered()
{
	controllables.sort(linkedComparator);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerReordered, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerReordered, this));
}

void PresetParameterContainer::parameterValueChanged(Parameter* source)
{
	ControllableContainer::parameterValueChanged(source);
	if (!keepValuesInSync) return;
	ParameterPreset* pp = getParameterPresetForSource(source);
	if (pp == nullptr) return;
	pp->parameter->setValue(source->value);
}

void PresetParameterContainer::parameterRangeChanged(Parameter* source)
{
	ControllableContainer::parameterRangeChanged(source);
	ParameterPreset* pp = getParameterPresetForSource(source);
	if (pp == nullptr) return;
	syncItem(pp, keepValuesInSync);
}

void PresetParameterContainer::controllableNameChanged(Controllable* sourceC)
{
	Parameter* source = dynamic_cast<Parameter*>(sourceC);
	if (source == nullptr) return;

	ParameterPreset* pp = getParameterPresetForSource(source);
	if (pp == nullptr) return;
	syncItem(pp, keepValuesInSync);
}

ParameterPreset* PresetParameterContainer::getParameterPresetForSource(Parameter* p)
{
	HashMap<ParameterPreset*, Parameter*>::Iterator i(linkMap);
	while (i.next()) if (p == i.getValue()) return i.getKey();
	return nullptr;
}

void PresetParameterContainer::loadJSONData(var data, bool createIfNotThere)
{
	resetAndBuildValues();
	ControllableContainer::loadJSONData(data, createIfNotThere);
}

ParameterPreset::ParameterPreset(Parameter* p) :
	ControllableContainer(p->niceName),
	parameter(p)
{
	addParameter(p);
	interpolationMode = addEnumParameter("Mode", "Interpolation mode, sets how interpolation is done");

	switch (p->type)
	{
	case Parameter::BOOL:
		break;

	case Parameter::STRING:
		break;

	case Parameter::ENUM:
		break;

	case Parameter::TARGET:
		break;

	case Parameter::CUSTOM:
		break;

	default:
		interpolationMode->addOption("Interpolate", INTERPOLATE);
		break;
	}

	interpolationMode->addOption("Change at start", CHANGE_AT_START)->addOption("Change at end", CHANGE_AT_END)->addOption("None", NONE);
}

ParameterPreset::~ParameterPreset()
{
}

InspectableEditor* ParameterPreset::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ParameterPresetEditor(this, isRoot);
}