/*
  ==============================================================================

	CVPreset.cpp
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"

CVPreset::CVPreset(CVGroup* group, bool isTemporary) :
	MorphTarget("Preset"),
	group(group),
	values("Values", &group->values, false, isTemporary)
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

var CVPreset::getJSONData(bool includeNonOverriden)
{
	var data = MorphTarget::getJSONData(includeNonOverriden);
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
	for (auto& cc : values.controllableContainers)
	{
		if (ParameterPreset* pp = dynamic_cast<ParameterPreset*>(cc.get()))
		{
			var ppData(new DynamicObject());
			ppData.getDynamicObject()->setProperty("value", pp->parameter->value);
			ppData.getDynamicObject()->setProperty("interpolationMode", pp->interpolationMode->getValueKey());
			data.getDynamicObject()->setProperty(pp->shortName, ppData);

		}
	}
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
		if (ParameterPreset* pp = dynamic_cast<ParameterPreset*>(values.getControllableContainerByName(nv.name.toString())))
		{
			if (nv.value.isObject() && nv.value.hasProperty("value"))
			{
				pp->parameter->setValue(nv.value.getProperty("value", var()));
				pp->interpolationMode->setValueWithKey(nv.value.getProperty("interpolationMode", var()));
			}
			else
			{
				pp->parameter->setValue(nv.value);
			}
			pp->setTimelineValueAuthored(true);
		}
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

PresetParameterContainer::PresetParameterContainer(const String& name, GenericControllableManager* manager, bool keepValuesInSync, bool doNotBuildValues) :
	ControllableContainer(name),
	manager(manager),
	keepValuesInSync(keepValuesInSync),
	linkedComparator(manager)
{
	saveAndLoadRecursiveData = true;

	manager->addBaseManagerListener(this);
	if (!doNotBuildValues) resetAndBuildValues(keepValuesInSync);
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
	const ScopedValueSetter<bool> metadataGuard(preset->syncingMetadata, true);
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

	if (onlyReturnUndoAction)
	{
		class TimelinePresetValueAction : public UndoableAction
		{
		public:
			TimelinePresetValueAction(ParameterPreset* pp, UndoableAction* action) : preset(pp), valueAction(action), wasAuthored(pp->hasTimelineValue) {}
			bool perform() override
			{
				if (!preset) return true;
				if (!valueAction || !valueAction->perform()) return false;
				static_cast<ParameterPreset*>(preset.get())->setTimelineValueAuthored(true);
				return true;
			}
			bool undo() override
			{
				if (!preset) return true;
				if (!valueAction || !valueAction->undo()) return false;
				static_cast<ParameterPreset*>(preset.get())->setTimelineValueAuthored(wasAuthored);
				return true;
			}
		private:
			WeakReference<ControllableContainer> preset;
			std::unique_ptr<UndoableAction> valueAction;
			bool wasAuthored;
		};
		auto* action = p->setUndoableValue(p->value, source->value, true);
		if (!action) { preset->setTimelineValueAuthored(true); return nullptr; }
		if (preset->hasTimelineValue) return action;
		return new TimelinePresetValueAction(preset, action);
	}

	p->setValue(source->value);
	if (!preset->syncingMetadata) preset->setTimelineValueAuthored(true);
	return nullptr;
}

void PresetParameterContainer::itemAdded(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
	if (auto* pp = getParameterPresetForSource(dynamic_cast<Parameter*>(gci->controllable))) pp->hasTimelineValue = false;
}

void PresetParameterContainer::itemsAdded(Array<GenericControllableItem*> items)
{
	for (auto& gci : items)
	{
		if (gci->controllable->type == Controllable::TRIGGER) continue;
		addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
		if (auto* pp = getParameterPresetForSource(dynamic_cast<Parameter*>(gci->controllable))) pp->hasTimelineValue = false;
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
	for (auto& cc : controllableContainers)
		if (auto* pp = dynamic_cast<ParameterPreset*>(cc.get())) pp->hasTimelineValue = false;
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

var ParameterPreset::getJSONData(bool includeNonOverriden)
{
	var data = ControllableContainer::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("timelineValueAuthored", hasTimelineValue);
	return data;
}

void ParameterPreset::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
	hasTimelineValue = data.getProperty("timelineValueAuthored", true);
}

void ParameterPreset::onContainerParameterChanged(Parameter* p)
{
	if (p == parameter && !syncingMetadata && !isCurrentlyLoadingData) setTimelineValueAuthored(true);
}

void ParameterPreset::setTimelineValueAuthored(bool authored)
{
	if (hasTimelineValue == authored) return;
	hasTimelineValue = authored;
	notifyStructureChanged();
}

InspectableEditor* ParameterPreset::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ParameterPresetEditor(this, isRoot);
}
