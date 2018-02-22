/*
  ==============================================================================

	CVPreset.cpp
	Created: 17 Feb 2018 10:15:38am
	Author:  Ben

  ==============================================================================
*/

#include "CVPreset.h"
#include "../CVGroup.h"

CVPreset::CVPreset(CVGroup * group) :
	BaseItem("Preset"),
	group(group),
	values(&group->values,"Values")
{
	jassert(group != nullptr);
	values.hideEditorHeader = true;
	values.editorCanBeCollapsed = false;

	isSelectable = false;
	showInspectorOnSelect = false;

	addChildControllableContainer(&values);
}

CVPreset::~CVPreset()
{
}

var CVPreset::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("values", values.getJSONData());
	return data;
}

void CVPreset::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	values.loadJSONData(data.getProperty("values", var()),true);
}


GenericControllableManagerLinkedContainer::GenericControllableManagerLinkedContainer(GenericControllableManager * manager, const String &name) :
	ControllableContainer(name),
	manager(manager),
	linkedComparator(manager)
{
	manager->addBaseManagerListener(this);
	resetAndBuildValues();
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

void GenericControllableManagerLinkedContainer::parameterRangeChanged(Parameter * source)
{
	Parameter * p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, false);
}

void GenericControllableManagerLinkedContainer::controllableNameChanged(Controllable * sourceC)
{
	Parameter * source = dynamic_cast<Parameter *>(sourceC);
	if (source == nullptr) return;

	Parameter * p = getParameterForSource(source);
	if (p == nullptr) return;
	syncItem(p, source, false);
}

Parameter * GenericControllableManagerLinkedContainer::getParameterForSource(Parameter * p)
{
	HashMap<Parameter *, Parameter *>::Iterator i(linkMap);
	while (i.next()) if (p == i.getValue()) return i.getKey();
	return nullptr;
}
