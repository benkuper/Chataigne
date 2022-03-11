/*
  ==============================================================================

	MultiplexList.cpp
	Created: 19 Dec 2020 12:00:35pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include "MultiplexList.h"

BaseMultiplexList::BaseMultiplexList(const String& name, var params) :
	BaseItem(name, false),
	listSize(0)
{
	showInspectorOnSelect = false;
	isSelectable = false;
}

BaseMultiplexList::~BaseMultiplexList()
{
}

void BaseMultiplexList::setSize(int size)
{
	if (size == listSize) return;
	listSize = size;
	updateControllablesSetup();
}

void BaseMultiplexList::updateControllablesSetup()
{
	while (list.size() > listSize)
	{
		Controllable* c = list[list.size() - 1];
		list.removeAllInstancesOf(c);
		removeControllable(c);
	}

	while (list.size() < listSize)
	{
		Controllable* c = createListControllable();
		c->isCustomizableByUser = true;
		c->setNiceName("#" + String(list.size() + 1));
		list.add(c);
		addControllable(c);
	}
}

Controllable* BaseMultiplexList::createListControllable()
{
	return ControllableFactory::createControllable(getTypeString());
}

var BaseMultiplexList::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("listSize", listSize);
	return data;
}

void BaseMultiplexList::loadJSONData(var data, bool createIfNotThere)
{
	setSize(data.getProperty("listSize", 0));
	loadJSONDataMultiplexInternal(data);
	BaseItem::loadJSONData(data, createIfNotThere);
}

void BaseMultiplexList::notifyItemUpdated(int multiplexIndex)
{
	jassert(multiplexIndex >= 0);
	listListeners.call(&MultiplexListListener::listItemUpdated, multiplexIndex);
}


InputValueMultiplexList::InputValueMultiplexList(var params) :
	BaseMultiplexList(getTypeString(), params)
{
}

InputValueMultiplexList::~InputValueMultiplexList()
{
	for (auto& c : inputControllables)
	{
		if (c == nullptr || c.wasObjectDeleted()) continue;
		if (c->type == c->TRIGGER) ((Trigger*)c.get())->removeTriggerListener(this);
		else((Parameter*)c.get())->removeParameterListener(this);
	}
}

void InputValueMultiplexList::updateControllablesSetup()
{
	while (list.size() > listSize)
	{
		int index = list.size() - 1;
		Controllable* c = list[index];
		list.removeAllInstancesOf(c);
		removeControllable(c);
		if (Controllable* c = inputControllables[index])
		{
			if (c->type == c->TRIGGER) ((Trigger*)c)->removeTriggerListener(this);
			else((Parameter*)c)->removeParameterListener(this);
		}
	}

	while (list.size() < listSize)
	{
		TargetParameter* tp = addTargetParameter("#" + String(list.size() + 1), "Input Value Target");
		tp->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
		tp->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
		tp->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;

		list.add(tp);
		inputControllables.add(nullptr);
	}
}

void InputValueMultiplexList::onContainerParameterChangedInternal(Parameter* p)
{
	int index = list.indexOf(p);
	if (index != -1)
	{
		if (Controllable* c = inputControllables[index])
		{
			if (c->type == c->TRIGGER) ((Trigger*)c)->removeTriggerListener(this);
			else((Parameter*)c)->removeParameterListener(this);
		}

		inputControllables.set(index, nullptr);

		if (Controllable* c = ((TargetParameter*)p)->target)
		{
			if (c->type == c->TRIGGER) ((Trigger*)c)->addTriggerListener(this);
			else((Parameter*)c)->addParameterListener(this);
			inputControllables.set(index, c);

			listListeners.call(&MultiplexListListener::listReferenceUpdated);
			notifyItemUpdated(index);
		}
	}
}

void InputValueMultiplexList::onExternalParameterRangeChanged(Parameter* p)
{
	if (inputControllables.indexOf(p) != -1) listListeners.call(&MultiplexListListener::listReferenceUpdated);
}

void InputValueMultiplexList::onExternalParameterValueChanged(Parameter* p)
{
	notifyItemUpdated(inputControllables.indexOf(p));
}

void InputValueMultiplexList::onExternalTriggerTriggered(Trigger* t)
{
	notifyItemUpdated(inputControllables.indexOf(t));
}

void InputValueMultiplexList::fillFromExpression(const String& s)
{
	for (int i = 0; i < listSize; i++)
	{
		String exp = s.replace("{index}", String(i + 1)).replace("{index0}", String(i));

		LOG("Exp(#" << (i + 1) << ") : " << exp);

		ScriptExpression e;
		e.setExpression(exp);
		if (e.state == ScriptExpression::EXPRESSION_LOADED)
		{
			e.evaluate();
			LOG(" > " << e.currentValue.toString());
			((TargetParameter*)list[i])->setValue(e.currentValue.toString());
		}
	}
}

InspectableEditor* InputValueMultiplexList::getEditorInternal(bool isRoot)
{
	return new InputValueListEditor(this, isRoot);
}

EnumMultiplexList::EnumMultiplexList(var params) :
	MultiplexList(EnumParameter::getTypeStringStatic() + " List")
{
}

EnumMultiplexList::~EnumMultiplexList()
{
}


void EnumMultiplexList::updateOption(int index, const String& key, const String& value)
{
	referenceOptions.set(index, new EnumParameter::EnumValue(key, value));
	updateControllablesSetup();
}

void EnumMultiplexList::cleanReferenceOptions()
{
	Array<EnumParameter::EnumValue*> evToRemove;
	for (auto& ev : referenceOptions) if (ev->key.isEmpty()) evToRemove.add(ev);
	for (auto& ev : evToRemove) referenceOptions.removeObject(ev);
}

void EnumMultiplexList::updateControllablesSetup()
{
	BaseMultiplexList::updateControllablesSetup();

	cleanReferenceOptions();
	for (auto& c : list)
	{
		EnumParameter* ep = (EnumParameter*)c;
		ep->setOptions(Array<EnumParameter::EnumValue*>(referenceOptions.getRawDataPointer(), referenceOptions.size()));
	}
}

void EnumMultiplexList::controllableAdded(Controllable* c)
{
	if (EnumParameter* ep = dynamic_cast<EnumParameter*>(c))
	{
		for (auto& ev : referenceOptions) ep->addOption(ev->key, ev->value);
	}

	MultiplexList::controllableAdded(c);
}

var EnumMultiplexList::getJSONData()
{
	var data = MultiplexList::getJSONData();
	var enumOptions(new DynamicObject());
	for (auto& ev : referenceOptions) enumOptions.getDynamicObject()->setProperty(ev->key, ev->value);
	data.getDynamicObject()->setProperty("enumOptions", enumOptions);
	return data;
}

void EnumMultiplexList::loadJSONDataMultiplexInternal(var data)
{
	if (data.hasProperty("enumOptions"))
	{
		NamedValueSet optionsData = data.getProperty("enumOptions", var()).getDynamicObject()->getProperties();
		for (auto& op : optionsData)
		{
			referenceOptions.add(new EnumParameter::EnumValue(op.name.toString(), op.value.toString()));
		}
	}

	updateControllablesSetup();
}

InspectableEditor* EnumMultiplexList::getEditorInternal(bool isRoot)
{
	return new EnumMultiplexListEditor(this, isRoot);
}

CVPresetMultiplexList::CVPresetMultiplexList(var params) :
	MultiplexList(getTypeStringStatic(), params)
{
	cvTarget = addTargetParameter("Target", "The Custom Variable Group to target for presets", CVGroupManager::getInstance());
	cvTarget->targetType = TargetParameter::CONTAINER;
	cvTarget->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetGroup;
	cvTarget->hideInEditor = true;
}

CVPresetMultiplexList::~CVPresetMultiplexList()
{
}

Controllable* CVPresetMultiplexList::createListControllable()
{
	return EnumParameter::create();
}

void CVPresetMultiplexList::controllableAdded(Controllable* c)
{
	if (EnumParameter* ep = dynamic_cast<EnumParameter*>(c)) updateItemList(ep);
}

void CVPresetMultiplexList::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == cvTarget)
	{
		for (auto& l : list) updateItemList((EnumParameter*)l);
	}
}

void CVPresetMultiplexList::updateItemList(EnumParameter* p)
{
	var v = p->getValueData();
	p->clearOptions();

	if (CVGroup* group = dynamic_cast<CVGroup*>(cvTarget->targetContainer.get()))
	{
		for (auto& preset : group->pm->items)
		{
			p->addOption(preset->niceName, preset->shortName);
		}
	}
}

CVPreset* CVPresetMultiplexList::getPresetAt(int multiplexIndex)
{
	EnumParameter* ep = (EnumParameter*)getTargetControllableAt(multiplexIndex);
	if (CVGroup* group = dynamic_cast<CVGroup*>(cvTarget->targetContainer.get())) return group->pm->getItemWithName(ep->getValueData());
	return nullptr;
}

Parameter* CVPresetMultiplexList::getPresetParameter(CVPreset* preset, const String& paramName)
{
	if (ParameterPreset* pp = dynamic_cast<ParameterPreset*>(preset->values.getControllableContainerByName(paramName))) return pp->parameter;

	return nullptr;
}

Parameter* CVPresetMultiplexList::getPresetParameterAt(int multiplexIndex, const String& paramName)
{
	if (CVPreset* preset = getPresetAt(multiplexIndex))  return getPresetParameter(preset, paramName);
	return nullptr;
}

InspectableEditor* CVPresetMultiplexList::getEditorInternal(bool isRoot)
{
	return new CVPresetMultiplexListEditor(this, isRoot);
}
