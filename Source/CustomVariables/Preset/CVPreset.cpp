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


	addChildControllableContainer(&values);
	
}

CVPreset::~CVPreset()
{
}

var CVPreset::getJSONData()
{
	var data = MorphTarget::getJSONData();
	data.getDynamicObject()->setProperty("values", values.getJSONData());
	return data;
}

void CVPreset::loadJSONDataInternal(var data)
{
	MorphTarget::loadJSONDataInternal(data);
	values.loadJSONData(data.getProperty("values", var()),true);
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




GenericControllableManagerLinkedContainer::GenericControllableManagerLinkedContainer(const String &name, GenericControllableManager * manager, bool keepValuesInSync) :
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

	HashMap<Parameter *, Parameter *>::Iterator i(linkMap);
	while (i.next())
	{
		i.getValue()->removeControllableListener(this);
		i.getValue()->removeParameterListener(this);
	}

}

void GenericControllableManagerLinkedContainer::resetAndBuildValues(bool syncValues)
{
	HashMap<Parameter *, Parameter *>::Iterator i(linkMap);
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

void GenericControllableManagerLinkedContainer::addValueFromItem(Parameter * source)
{
	if (source == nullptr) return;

	Controllable * c = ControllableFactory::createControllable(source->getTypeString());
	Parameter * p = dynamic_cast<Parameter *>(c);
	linkMap.set(p, source);
	source->addControllableListener(this);
	source->addParameterListener(this);
	syncItem(p, source);
	addParameter(p);
}

void GenericControllableManagerLinkedContainer::syncItem(Parameter * p, Parameter * source, bool syncValue)
{
	p->setNiceName(source->niceName);
	p->setRange(source->minimumValue, source->maximumValue);
	if (syncValue) p->setValue(source->value);
}

void GenericControllableManagerLinkedContainer::syncItems(bool syncValues)
{
	Array<WeakReference<Parameter>> pList = getAllParameters();
	for (auto &p : pList)
	{
		syncItem(p, linkMap[p], syncValues);
	}
}

void GenericControllableManagerLinkedContainer::itemAdded(GenericControllableItem * gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	addValueFromItem(dynamic_cast<Parameter *>(gci->controllable));
}

void GenericControllableManagerLinkedContainer::itemRemoved(GenericControllableItem * gci)
{
	if (gci->controllable->type == Controllable::TRIGGER) return;
	Parameter * p = dynamic_cast<Parameter *>(getControllableByName(gci->niceName, true));
	if (p != nullptr)
	{ 
		linkMap[p]->removeControllableListener(this);
		linkMap[p]->removeParameterListener(this);
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

void GenericControllableManagerLinkedContainer::parameterValueChanged(Parameter * source)
{
	ControllableContainer::parameterValueChanged(source);
	if (!keepValuesInSync) return;
	Parameter * p = getParameterForSource(source);
	if (p == nullptr) return;
	p->setValue(source->value);
}

void GenericControllableManagerLinkedContainer::parameterRangeChanged(Parameter * source)
{
	ControllableContainer::parameterRangeChanged(source);
	Parameter * p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, keepValuesInSync);
}

void GenericControllableManagerLinkedContainer::controllableNameChanged(Controllable * sourceC)
{
	Parameter * source = dynamic_cast<Parameter *>(sourceC);
	if (source == nullptr) return;

	Parameter * p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, keepValuesInSync);
}

Parameter * GenericControllableManagerLinkedContainer::getParameterForSource(Parameter * p)
{
	HashMap<Parameter *, Parameter *>::Iterator i(linkMap);
	while (i.next()) if (p == i.getValue()) return i.getKey();
	return nullptr;
}

