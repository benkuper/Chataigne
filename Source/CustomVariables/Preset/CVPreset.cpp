/*
  ==============================================================================

	CVPreset.cpp
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#include "CVPreset.h"
#include "../CVGroup.h"
#include "ui/CVPresetEditor.h"

CVPreset::CVPreset(CVGroup * group) :
	MorphTarget("Preset"),
	group(group),
	values("Values",&group->values,false)
{
	jassert(group != nullptr);


	values.hideEditorHeader = true;
	values.editorCanBeCollapsed = false;

	showInspectorOnSelect = false;

	CVGroup::ControlMode cm = group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>();

	weight->setControllableFeedbackOnly(cm == CVGroup::FREE || cm == CVGroup::WEIGHTS);

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
	values.loadJSONData(data.getProperty(values.shortName, var()),true);
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

InspectableEditor * CVPreset::getEditor(bool isRoot)
{
	return new CVPresetEditor(this, isRoot);
}




PresetParameterContainer::PresetParameterContainer(const String &name, GenericControllableManager * manager, bool keepValuesInSync) :
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

	HashMap<ParameterPreset *, Parameter *>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

}

void PresetParameterContainer::resetAndBuildValues(bool syncValues)
{
	HashMap<ParameterPreset *, Parameter *>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

	clear();
	for (auto &gci : manager->items)
	{
		addValueFromItem(dynamic_cast<Parameter *>(gci->controllable));
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
	syncItem(p, source);

	addChildControllableContainer(pp, true);
}

void PresetParameterContainer::syncItem(Parameter* p, Parameter* source, bool syncValue)
{
	p->setNiceName(source->niceName);
	p->setRange(source->minimumValue, source->maximumValue);
	if (syncValue) p->setValue(source->value);
}

void PresetParameterContainer::syncItems(bool syncValues)
{
	for (auto& cc : controllableContainers)
	{
		if (ParameterPreset* pp = dynamic_cast<ParameterPreset *>(cc.get()))
		{
			syncItem(pp->parameter, linkMap[pp], syncValues);
		}
	}
}

void PresetParameterContainer::itemAdded(GenericControllableItem* gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	addValueFromItem(dynamic_cast<Parameter*>(gci->controllable));
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
	syncItem(pp->parameter, source, keepValuesInSync);
}

void PresetParameterContainer::controllableNameChanged(Controllable* sourceC)
{
	Parameter* source = dynamic_cast<Parameter*>(sourceC);
	if (source == nullptr) return;

	ParameterPreset* pp = getParameterPresetForSource(source);
	if (pp == nullptr) return;
	syncItem(pp->parameter, source, keepValuesInSync);
}

ParameterPreset * PresetParameterContainer::getParameterPresetForSource(Parameter * p)
{
	HashMap<ParameterPreset *, Parameter *>::Iterator i(linkMap);
	while (i.next()) if (p == i.getValue()) return i.getKey();
	return nullptr;
}

ParameterPreset::ParameterPreset(Parameter * p) :
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

	default:
		interpolationMode->addOption("Interpolate", INTERPOLATE);
		break;
	}

	interpolationMode->addOption("Keep Start", START)->addOption("Keep End", END);
}

ParameterPreset::~ParameterPreset()
{

}

InspectableEditor* ParameterPreset::getEditor(bool isRoot)
{
	return new ParameterPresetEditor(this, isRoot);
}
