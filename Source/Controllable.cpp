/*
  ==============================================================================

    Controllable.cpp
    Created: 8 Mar 2016 1:08:56pm
    Author:  bkupe

  ==============================================================================
*/

#include "Controllable.h"
#include "ControllableContainer.h"
//#include "JsHelpers.h"
#include "StringUtil.h"
#include "ControllableEditor.h"

Controllable::Controllable(const Type &type, const String & niceName, const String &description, bool enabled) :
	ScriptTarget("",this),
    type(type),
description(description),
hasCustomShortName(false),
    isControllableExposed(true),
    isControllableFeedbackOnly(false),
    hideInEditor(false),
	hideInOutliner(false),
isTargettable(true),
	isSavable(true),
	saveValueOnly(true),
	isCustomizableByUser(false),
isRemovableByUser(false),
replaceSlashesInShortName(true),
parentContainer(nullptr)
{

    setEnabled(enabled);
    setNiceName(niceName);
}

Controllable::~Controllable() {
	Controllable::masterReference.clear();
	listeners.call(&Controllable::Listener::controllableRemoved, this);
}

void Controllable::setNiceName(const String & _niceName) {
	if (niceName == _niceName) return;

	this->niceName = _niceName;
	if (!hasCustomShortName) setAutoShortName();
	else listeners.call(&Listener::controllableNameChanged, this);
}

void Controllable::setCustomShortName(const String & _shortName)
{
	this->shortName = _shortName;
	hasCustomShortName = true;
	updateControlAddress();
	listeners.call(&Listener::controllableNameChanged, this);
	scriptTargetName = shortName;
}

void Controllable::setAutoShortName() {
	hasCustomShortName = false;
	shortName = StringUtil::toShortName(niceName, replaceSlashesInShortName);
	updateControlAddress();
	listeners.call(&Listener::controllableNameChanged, this);
	scriptTargetName = shortName;
}

void Controllable::setEnabled(bool value, bool silentSet, bool force)
{
	if (!force && value == enabled) return;

	enabled = value;
	if (!silentSet) listeners.call(&Listener::controllableStateChanged, this);
}

void Controllable::setParentContainer(ControllableContainer * container)
{
	this->parentContainer = container;
	updateControlAddress();
}

void Controllable::updateControlAddress()
{
	this->controlAddress = getControlAddress();
	listeners.call(&Listener::controllableControlAddressChanged, this); 
}

DynamicObject * Controllable::createScriptObject()
{
	DynamicObject * o = ScriptTarget::createScriptObject();
	o->setProperty("name", shortName);
	o->setProperty("niceName", niceName);

	return o;
}

void Controllable::remove()
{
	listeners.call(&Controllable::Listener::askForRemoveControllable, this);
}

var Controllable::getJSONData(ControllableContainer * relativeTo)
{
	var data = getJSONDataInternal();
	data.getDynamicObject()->setProperty("controlAddress", getControlAddress(relativeTo));
	
	if(saveValueOnly) return data;
	
	data.getDynamicObject()->setProperty("type", getTypeString());
	data.getDynamicObject()->setProperty("niceName", niceName);
	data.getDynamicObject()->setProperty("customizable", isCustomizableByUser);
	data.getDynamicObject()->setProperty("removable", isRemovableByUser);

	if (hasCustomShortName) data.getDynamicObject()->setProperty("shortName", shortName);

	return data;
}

var Controllable::getJSONDataInternal() 
{
	return var(new DynamicObject());
}

void Controllable::loadJSONData(var data)
{
	if (data.getDynamicObject()->hasProperty("niceName")) setNiceName(data.getProperty("niceName", ""));
	if (data.getDynamicObject()->hasProperty("shortName")) setCustomShortName(data.getProperty("shortName", ""));	
	if (data.getDynamicObject()->hasProperty("customizable")) isCustomizableByUser = data.getProperty("customizable", false);
	if (data.getDynamicObject()->hasProperty("removable")) isRemovableByUser = data.getProperty("removable",false);

	loadJSONDataInternal(data);
}

String Controllable::getControlAddress(ControllableContainer * relativeTo)
{
  // we may need empty parentContainer in unit tests
#if LGML_UNIT_TESTS
  return (parentContainer?parentContainer->getControlAddress(relativeTo):"") + "/"+shortName;
#else
  return parentContainer->getControlAddress(relativeTo) + "/"+shortName;
#endif
}


InspectableEditor * Controllable::getEditor(bool isRootEditor) {

	return new ControllableEditor(this,isRootEditor); 
}


//SCRIPT

var Controllable::setValueFromScript(const juce::var::NativeFunctionArgs& a) {

	Controllable * c = getObjectFromJS<Controllable>(a);
	bool success = false;

	if (c != nullptr)
	{

		success = true;

		if (a.numArguments == 0 && c->type == Controllable::Type::TRIGGER) {
			((Trigger *)c)->trigger();
		}

		else {
			var value = a.arguments[0];
			bool valueIsABool = value.isBool();
			bool valueIsANumber = value.isDouble() || value.isInt() || value.isInt64();

			switch (c->type)
			{
			case Controllable::Type::TRIGGER:
				if (valueIsABool)
				{
					if ((bool)value) ((Trigger *)c)->trigger();
				}
				else if (valueIsANumber)
				{
					if ((float)value >= 1) ((Trigger *)c)->trigger();
				}

				break;

			case Controllable::Type::BOOL:

				if (valueIsABool) ((BoolParameter *)c)->setValue((bool)value);
				else if (valueIsANumber) ((BoolParameter *)c)->setValue((float)value > .5);
				break;

			case Controllable::Type::FLOAT:
				if (valueIsABool || valueIsANumber) ((FloatParameter *)c)->setValue(value);
				break;
			case Controllable::Type::INT:
				if (valueIsABool || valueIsANumber) ((IntParameter *)c)->setValue(value);
				break;


			case Controllable::Type::STRING:
				if (value.isString()) ((StringParameter *)c)->setValue(value);
				break;

			default:
				success = false;
				break;

			}
		}
	}

	return var();
}
