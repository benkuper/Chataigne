/*
  ==============================================================================

	MultiplexList.cpp
	Created: 19 Dec 2020 12:00:35pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"

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

void BaseMultiplexList::fillFromExpression(const String& s, int start, int end)
{
	for (int i = jmax(start - 1, 0); i < end; i++)
	{
		String exp = s.replace("{index}", String(i + 1)).replace("{index0}", String(i));

		LOG("Exp(#" << (i + 1) << ") : " << exp);

		ScriptExpression e;
		e.setExpression(exp);
		if (e.state == ScriptExpression::EXPRESSION_LOADED)
		{
			e.evaluate();
			LOG(" > " << e.currentValue.toString());

			if (list[i]->type == Controllable::TARGET)
			{
				if (!e.currentValue.isString() || e.currentValue.toString().isEmpty())
				{
					((TargetParameter*)list[i])->resetValue();
				}
				else if (e.currentValue.toString() != "{value}")
				{
					((TargetParameter*)list[i])->setValue(e.currentValue.toString());
				}
			}
			else
			{
				if (!e.currentValue.isVoid())
				{
					if (list[i]->type != Controllable::TRIGGER) ((Parameter*)list[i])->setValue(e.currentValue);
				}
			}
		}
	}
}

Controllable* BaseMultiplexList::createListControllable()
{
	return ControllableFactory::createControllable(getTypeString());
}

InspectableEditor* BaseMultiplexList::getNumberListEditor(bool isFloat, bool isRoot, Array<Inspectable*> inspectable)
{
	if (isFloat) return new NumberListEditor((MultiplexList<FloatParameter> *)this, isRoot);
	else return new NumberListEditor((MultiplexList<IntParameter> *)this, isRoot);
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

		if (Controllable* ic = inputControllables[index])
		{
			if (controllableIndexMap.contains(ic))
			{
				controllableIndexMap.getReference(ic).removeAllInstancesOf(index);
				if (controllableIndexMap.getReference(ic).isEmpty()) controllableIndexMap.remove(ic);
			}

			if (!controllableIndexMap.contains(ic))
			{
				if (ic->type == ic->TRIGGER) ((Trigger*)ic)->removeTriggerListener(this);
				else((Parameter*)ic)->removeParameterListener(this);
			}
		}

		inputControllables.removeLast();
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
		//Find previous input controllable and remove the map and listener if needed
		if (Controllable* ic = inputControllables[index])
		{
			if (controllableIndexMap.contains(ic))
			{
				controllableIndexMap.getReference(ic).removeAllInstancesOf(index);
				if (controllableIndexMap.getReference(ic).isEmpty()) controllableIndexMap.remove(ic);
			}


			if (!controllableIndexMap.contains(ic))
			{
				if (ic->type == ic->TRIGGER) ((Trigger*)ic)->removeTriggerListener(this);
				else((Parameter*)ic)->removeParameterListener(this);
			}
		}

		inputControllables.set(index, nullptr);

		if (Controllable* c = ((TargetParameter*)p)->target)
		{

			if (c->type == c->TRIGGER) ((Trigger*)c)->addTriggerListener(this);
			else((Parameter*)c)->addParameterListener(this);
			inputControllables.set(index, c);

			if (!controllableIndexMap.contains(c)) controllableIndexMap.set(c, Array<int>(index));
			else controllableIndexMap.getReference(c).add(index);

		}

		listListeners.call(&MultiplexListListener::listReferenceUpdated);
		notifyItemUpdated(index);
	}
}

void InputValueMultiplexList::onExternalParameterRangeChanged(Parameter* p)
{
	if (inputControllables.indexOf(p) != -1) listListeners.call(&MultiplexListListener::listReferenceUpdated);
}

void InputValueMultiplexList::onExternalParameterValueChanged(Parameter* p)
{
	jassert(controllableIndexMap.contains(p));
	for (auto& i : controllableIndexMap[p]) notifyItemUpdated(i);
	//notifyItemUpdated(inputControllables.indexOf(p));
}

void InputValueMultiplexList::onExternalTriggerTriggered(Trigger* t)
{
	jassert(controllableIndexMap.contains(t));
	for (auto& i : controllableIndexMap[t]) notifyItemUpdated(i);
}



InspectableEditor* InputValueMultiplexList::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
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

InspectableEditor* EnumMultiplexList::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new EnumMultiplexListEditor(this, isRoot);
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


TargetMultiplexList::TargetMultiplexList(var params) :
	MultiplexList(TargetParameter::getTypeStringStatic() + " List"),
	containerMode(false)
{
}

TargetMultiplexList::~TargetMultiplexList()
{
}


void TargetMultiplexList::setContainerMode(bool value)
{
	containerMode = value;
	updateControllablesSetup();
}


void TargetMultiplexList::updateControllablesSetup()
{
	BaseMultiplexList::updateControllablesSetup();

	for (auto& c : list)
	{
		TargetParameter* ep = (TargetParameter*)c;
		ep->targetType = containerMode ? TargetParameter::CONTAINER : TargetParameter::CONTROLLABLE;
	}
}

void TargetMultiplexList::controllableAdded(Controllable* c)
{
	((TargetParameter*)c)->targetType = containerMode ? TargetParameter::CONTAINER : TargetParameter::CONTROLLABLE;
	MultiplexList::controllableAdded(c);
}

InspectableEditor* TargetMultiplexList::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new TargetMultiplexListEditor(this, isRoot);
}

var TargetMultiplexList::getJSONData()
{
	var data = MultiplexList::getJSONData();
	data.getDynamicObject()->setProperty("containerMode", containerMode);
	return data;
}

void TargetMultiplexList::loadJSONDataMultiplexInternal(var data)
{
	setContainerMode((bool)data.getProperty("containerMode", containerMode));
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

InspectableEditor* CVPresetMultiplexList::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CVPresetMultiplexListEditor(this, isRoot);
}
