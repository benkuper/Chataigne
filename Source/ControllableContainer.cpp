/*
 ==============================================================================

 ControllableContainer.cpp
 Created: 8 Mar 2016 1:15:36pm
 Author:  bkupe

 ==============================================================================
 */

#include "ControllableContainer.h"

#include "ControllableUI.h"
#include "DebugHelpers.h"
#include "StringUtil.h"
#include "ControllableFactory.h"
#include "TargetParameter.h"

#include "GenericControllableContainerEditor.h"


ControllableComparator ControllableContainer::comparator;

ControllableContainer::ControllableContainer(const String & niceName) :
	ScriptTarget("", this),
	parentContainer(nullptr),
	hasCustomShortName(false),
	skipControllableNameInAddress(false),
	currentPreset(nullptr),
	canHavePresets(false),
	saveAndLoadRecursiveData(false),
	numContainerIndexed(0),
	localIndexedPosition(-1),
	presetSavingIsRecursive(false),
	saveAndLoadName(false),
	nameCanBeChangedByUser(true),
	isTargettable(true),
	hideInEditor(false),
	canInspectChildContainers(true),
	includeInScriptObject(true),
	queuedNotifier(500) //what to put in max size ?? 
						//500 seems ok on my compute, but if too low, generates leaks when closing app while heavy use of async (like  parameter update from audio signal)
{
	setNiceName(niceName);

	if (canHavePresets)
	{
		currentPresetName = addStringParameter("Preset", "Current Preset", "");
		currentPresetName->hideInEditor = true;
		currentPresetName->hideInOutliner = true;
		currentPresetName->isTargettable = false;

		savePresetTrigger = addTrigger("Save Preset", "Save current preset");
		savePresetTrigger->hideInEditor = true;
		savePresetTrigger->isTargettable = false;
	}

	//script
	scriptObject.setMethod("getChild", ControllableContainer::getChildFromScript);
}

ControllableContainer::~ControllableContainer()
{
	//controllables.clear();
	//DBG("CLEAR CONTROLLABLE CONTAINER");
	clear();
	masterReference.clear();
}


void ControllableContainer::clear() {

	controllables.clear();
	controllableContainers.clear();
	queuedNotifier.cancelPendingUpdate();
}


void ControllableContainer::addControllable(Controllable * c)
{
	if (c->type == Controllable::TRIGGER) addTriggerInternal((Trigger *)c);
	else addParameterInternal((Parameter *)c);
	c->addControllableListener(this);
}

void ControllableContainer::addParameter(Parameter * p)
{
	addControllable(p);
}

FloatParameter * ControllableContainer::addFloatParameter(const String & _niceName, const String & description, const float & initialValue, const float & minValue, const float & maxValue, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	FloatParameter * p = new FloatParameter(targetName, description, initialValue, minValue, maxValue, enabled);
	addControllable(p);
	return p;
}

IntParameter * ControllableContainer::addIntParameter(const String & _niceName, const String & _description, const int & initialValue, const int & minValue, const int & maxValue, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	IntParameter * p = new IntParameter(targetName, _description, initialValue, minValue, maxValue, enabled);
	addControllable(p);
	return p;
}

BoolParameter * ControllableContainer::addBoolParameter(const String & _niceName, const String & _description, const bool & value, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	BoolParameter * p = new BoolParameter(targetName, _description, value, enabled);
	addControllable(p);
	return p;
}

StringParameter * ControllableContainer::addStringParameter(const String & _niceName, const String & _description, const String &value, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	StringParameter * p = new StringParameter(targetName, _description, value, enabled);
	addControllable(p);
	return p;
}

EnumParameter * ControllableContainer::addEnumParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	EnumParameter * p = new EnumParameter(targetName, _description, enabled);
	addControllable(p);
	return p;
}

Point2DParameter * ControllableContainer::addPoint2DParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	Point2DParameter * p = new Point2DParameter(targetName, _description, enabled);
	addControllable(p);
	return p;
}

Point3DParameter * ControllableContainer::addPoint3DParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	Point3DParameter * p = new Point3DParameter(targetName, _description, enabled);
	addControllable(p);
	return p;
}

TargetParameter * ControllableContainer::addTargetParameter(const String & _niceName, const String & _description, WeakReference<ControllableContainer> rootReference, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	TargetParameter * p = new TargetParameter(targetName, _description, "", rootReference, enabled);
	addControllable(p);
	return p;
}

Trigger * ControllableContainer::addTrigger(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	Trigger * t = new Trigger(targetName, _description, enabled);
	addControllable(t);
	return t;
}


void ControllableContainer::addTriggerInternal(Trigger * t)
{
	controllables.add(t);
	t->setParentContainer(this);
	t->addTriggerListener(this);

	controllableContainerListeners.call(&ControllableContainerListener::controllableAdded, t);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableAdded, this, t));
	notifyStructureChanged();
}

void ControllableContainer::addParameterInternal(Parameter * p)
{
	p->setParentContainer(this);
	controllables.add(p);
	p->addParameterListener(this);
	p->addAsyncParameterListener(this);
	controllableContainerListeners.call(&ControllableContainerListener::controllableAdded, p);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableAdded, this, p));
	notifyStructureChanged();
}

void ControllableContainer::removeControllable(Controllable * c)
{
	controllableContainerListeners.call(&ControllableContainerListener::controllableRemoved, c);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableRemoved, this, c));

	if (Parameter * p = dynamic_cast<Parameter*>(c)) {
		p->removeParameterListener(this);
		p->removeAsyncParameterListener(this);
	}

	c->removeControllableListener(this);

	controllables.removeObject(c);
	notifyStructureChanged();
}


void ControllableContainer::notifyStructureChanged() {

	controllableContainerListeners.call(&ControllableContainerListener::childStructureChanged, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ChildStructureChanged, this));

}

void ControllableContainer::newMessage(const Parameter::ParamWithValue& pv) {
	if (pv.parameter == currentPresetName) {
		loadPresetWithName(pv.parameter->stringValue());
	}
	if (!pv.isRange()) {
		onContainerParameterChangedAsync(pv.parameter, pv.value);
	}
}
void ControllableContainer::setNiceName(const String &_niceName) {
	if (niceName == _niceName) return;
	niceName = _niceName;
	if (!hasCustomShortName) setAutoShortName();
	onContainerNiceNameChanged();
}

void ControllableContainer::setCustomShortName(const String &_shortName) {
	shortName = _shortName;
	hasCustomShortName = true;
	scriptTargetName = shortName;
	updateChildrenControlAddress();
	onContainerShortNameChanged();
	controllableContainerListeners.call(&ControllableContainerListener::childAddressChanged, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ChildAddressChanged, this));

}

void ControllableContainer::setAutoShortName() {
	hasCustomShortName = false;
	shortName = StringUtil::toShortName(niceName);
	scriptTargetName = shortName;
	updateChildrenControlAddress();
	onContainerShortNameChanged();
	controllableContainerListeners.call(&ControllableContainerListener::childAddressChanged, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ChildAddressChanged, this));
}

void ControllableContainer::setCanHavePresets(bool value)
{
	canHavePresets = value;

	//TODO, create triggers and string parameter here instead of having them everywhere
	//currentPresetName->isControllableExposed = value;
}



Controllable * ControllableContainer::getControllableByName(const String & name, bool searchNiceNameToo)
{
	for (auto &c : controllables)
	{
		if (c->shortName == name || (searchNiceNameToo && c->niceName == name)) return c;
	}

	return nullptr;
}

void ControllableContainer::addChildControllableContainer(ControllableContainer * container)
{

	String targetName = getUniqueNameInContainer(container->niceName);
	container->setNiceName(targetName);

	controllableContainers.add(container);
	container->addControllableContainerListener(this);
	container->setParentContainer(this);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerAdded, container);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerAdded, this, container));

	notifyStructureChanged();
}

void ControllableContainer::removeChildControllableContainer(ControllableContainer * container)
{
	if (numContainerIndexed > 0 &&
		container->localIndexedPosition >= 0 &&
		controllableContainers.getUnchecked(container->localIndexedPosition) == container) {
		numContainerIndexed--;
	}
	this->controllableContainers.removeAllInstancesOf(container);
	container->removeControllableContainerListener(this);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerRemoved, container);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerRemoved, this, container));

	notifyStructureChanged();
	container->setParentContainer(nullptr);
}

void ControllableContainer::addChildIndexedControllableContainer(ControllableContainer * container, int idx) {
	if (idx == -1)idx = numContainerIndexed;
	jassert(idx <= numContainerIndexed);

	controllableContainers.insert(idx, container);
	container->localIndexedPosition = idx;
	numContainerIndexed++;

	container->addControllableContainerListener(this);
	container->setParentContainer(this);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerAdded, container);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerAdded, this, container));

	notifyStructureChanged();
}

void ControllableContainer::removeChildIndexedControllableContainer(int idx) {
	if (idx == -1)idx = numContainerIndexed - 1;
	jassert(idx < numContainerIndexed);


	removeChildControllableContainer(controllableContainers.getUnchecked(idx));
	numContainerIndexed--;

	for (int i = idx; i < numContainerIndexed; i++) {
		controllableContainers.getUnchecked(i)->localIndexedPosition = i;
		controllableContainers.getUnchecked(i)->localIndexChanged();
	}

}

int ControllableContainer::getNumberOfIndexedContainer() { return numContainerIndexed; }
int ControllableContainer::getIndexedPosition() { return localIndexedPosition; }
bool ControllableContainer::hasIndexedContainers() { return numContainerIndexed > 0; }
bool ControllableContainer::isIndexedContainer() { return localIndexedPosition >= 0; }
void ControllableContainer::localIndexChanged() {};

ControllableContainer * ControllableContainer::getControllableContainerByName(const String & name, bool searchNiceNameToo)
{
	for (auto &cc : controllableContainers)
	{
		if (cc.get() && (cc->shortName == name || (searchNiceNameToo && cc->niceName == name))) return cc;
	}

	return nullptr;

}

ControllableContainer * ControllableContainer::getControllableContainerForAddress(const String&  address, bool recursive, bool getNotExposed)
{
	StringArray addrArray;
	addrArray.addTokens(address, juce::StringRef("/"), juce::StringRef("\""));
	addrArray.remove(0);

	return getControllableContainerForAddress(addrArray, recursive, getNotExposed);
}

ControllableContainer * ControllableContainer::getControllableContainerForAddress(StringArray  addressSplit, bool recursive, bool getNotExposed)
{

	if (addressSplit.size() == 0) jassertfalse; // SHOULD NEVER BE THERE !

	bool isTargetFinal = addressSplit.size() == 1;

	if (isTargetFinal)
	{

		if (ControllableContainer * res = getControllableContainerByName(addressSplit[0]))   //get not exposed here here ?
			return res;

		//no found in direct children Container, maybe in a skip container ?
		for (auto &cc : controllableContainers)
		{
			if (cc->skipControllableNameInAddress)
			{
				if (ControllableContainer * res = cc->getControllableContainerForAddress(addressSplit, recursive, getNotExposed)) return res;
			}
		}
	} else //if recursive here ?
	{
		for (auto &cc : controllableContainers)
		{

			if (!cc->skipControllableNameInAddress)
			{
				if (cc->shortName == addressSplit[0])
				{
					addressSplit.remove(0);
					return cc->getControllableContainerForAddress(addressSplit, recursive, getNotExposed);
				}
			} else
			{
				ControllableContainer * tc = cc->getControllableContainerByName(addressSplit[0]);
				if (tc != nullptr)
				{
					addressSplit.remove(0);
					return tc->getControllableContainerForAddress(addressSplit, recursive, getNotExposed);
				}

			}
		}
	}

	return nullptr;

}

String ControllableContainer::getControlAddress(ControllableContainer * relativeTo) {

	StringArray addressArray;
	ControllableContainer * pc = this;
	while (pc != relativeTo && pc != nullptr)
	{
		if (!pc->skipControllableNameInAddress) addressArray.insert(0, pc->shortName);
		pc = pc->parentContainer;
	}
	if (addressArray.size() == 0)return "";
	else return "/" + addressArray.joinIntoString("/");
}

void ControllableContainer::orderControllablesAlphabetically()
{
	controllables.sort(ControllableContainer::comparator, true);
	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerReordered, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerReordered, this));

}

void ControllableContainer::setParentContainer(ControllableContainer * container)
{
	this->parentContainer = container;
	for (auto &c : controllables) c->updateControlAddress();
	for (auto &cc : controllableContainers) cc->updateChildrenControlAddress();

}

void ControllableContainer::updateChildrenControlAddress()
{
	for (auto &c : controllables) c->updateControlAddress();
	for (auto &cc : controllableContainers) cc->updateChildrenControlAddress();


}

Array<WeakReference<Controllable>> ControllableContainer::getAllControllables(bool recursive, bool getNotExposed)
{
	Array<WeakReference<Controllable>> result;
	for (auto &c : controllables)
	{
		if (getNotExposed || c->isControllableExposed) result.add(c);
	}

	if (recursive)
	{
		for (auto &cc : controllableContainers) result.addArray(cc->getAllControllables(true, getNotExposed));
	}

	return result;
}

Array<WeakReference<Parameter>> ControllableContainer::getAllParameters(bool recursive, bool getNotExposed)
{
	Array<WeakReference<Parameter>> result;

	for (auto &c : controllables)
	{
		if (c->type == Controllable::Type::TRIGGER) continue;
		if (getNotExposed || c->isControllableExposed) {
			if (Parameter * cc = dynamic_cast<Parameter*>(c)) {
				result.add(cc);
			}
		}
	}

	if (recursive)
	{
		for (auto &cc : controllableContainers) result.addArray(cc->getAllParameters(true, getNotExposed));
	}

	return result;
}



Controllable * ControllableContainer::getControllableForAddress(const String &address, bool recursive, bool getNotExposed)
{
	StringArray addrArray;
	addrArray.addTokens(address, juce::StringRef("/"), juce::StringRef("\""));
	addrArray.remove(0);

	return getControllableForAddress(addrArray, recursive, getNotExposed);
}

Controllable * ControllableContainer::getControllableForAddress(StringArray addressSplit, bool recursive, bool getNotExposed)
{
	if (addressSplit.size() == 0) jassertfalse; // SHOULD NEVER BE THERE !

	bool isTargetAControllable = addressSplit.size() == 1;

	if (isTargetAControllable)
	{
		//DBG("Check controllable Address : " + shortName);
		for (auto &c : controllables)
		{
			if (c->shortName == addressSplit[0])
			{
				//DBG(c->shortName);
				if (c->isControllableExposed || getNotExposed) return c;
				else return nullptr;
			}
		}

		//no found in direct children controllables, maybe in a skip container ?
		for (auto &cc : controllableContainers)
		{
			if (cc.wasObjectDeleted()) continue;
			if (cc->skipControllableNameInAddress)
			{
				Controllable * tc = cc->getControllableByName(addressSplit[0]);  //get not exposed here here
				if (tc != nullptr) return tc;
			}
		}
	} else  //if recursive here ?
	{
		for (auto &cc : controllableContainers)
		{
			if (cc.wasObjectDeleted()) continue;
			if (!cc->skipControllableNameInAddress)
			{
				if (cc->shortName == addressSplit[0])
				{
					addressSplit.remove(0);
					return cc->getControllableForAddress(addressSplit, recursive, getNotExposed);
				}
			} else
			{
				ControllableContainer * tc = cc->getControllableContainerByName(addressSplit[0]);
				if (tc != nullptr)
				{
					addressSplit.remove(0);
					return tc->getControllableForAddress(addressSplit, recursive, getNotExposed);
				}

			}
		}
	}

	return nullptr;
}

bool ControllableContainer::containsControllable(Controllable * c, int maxSearchLevels)
{
	if (c == nullptr) return false;

	ControllableContainer * pc = c->parentContainer;
	if (pc == nullptr) return false;
	int curLevel = 0;

	while (pc != nullptr)
	{
		if (pc == this) return true;
		curLevel++;
		if (maxSearchLevels >= 0 && curLevel > maxSearchLevels) return false;
		pc = pc->parentContainer;
	}

	return false;
}


bool ControllableContainer::loadPresetWithName(const String & name)
{
	// TODO weird feedback when loading preset on parameter presetName
	if (isLoadingPreset) { return false; }
	if (name == "") return false;
	isLoadingPreset = true;

	PresetManager::Preset * preset = PresetManager::getInstance()->getPreset(getPresetFilter(), name);
	if (preset == nullptr) { isLoadingPreset = false; currentPresetName->setValue("", true); return false; }
	bool hasLoaded = loadPreset(preset);
	isLoadingPreset = false;
	return hasLoaded;

}

bool ControllableContainer::loadPreset(PresetManager::Preset * preset)
{
	if (preset == nullptr) {
		currentPresetName->setValue("", true);
		return false;
	}

	loadPresetInternal(preset);

	for (auto &pv : preset->presetValues)
	{

		Parameter * p = dynamic_cast<Parameter *>(getControllableForAddress(pv->paramControlAddress));
		//DBG("Load preset, param set container : " << niceName << ", niceName : " << p->niceName << ",pv controlAddress : " << p->controlAddress << "" << pv->presetValue.toString());
		if (p != nullptr && p != currentPresetName) p->setValue(pv->presetValue);
	}

	currentPreset = preset;
	currentPresetName->setValue(currentPreset->name, false);

	controllableContainerListeners.call(&ControllableContainerListener::controllableContainerPresetLoaded, this);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerPresetLoaded, this));

	return true;
}

PresetManager::Preset* ControllableContainer::saveNewPreset(const String & _name)
{
	PresetManager::Preset * pre = PresetManager::getInstance()->addPresetFromControllableContainer(_name, getPresetFilter(), this, presetSavingIsRecursive);
	savePresetInternal(pre);
	NLOG(niceName, "New preset saved : " + pre->name);
	loadPreset(pre);
	return pre;
}


bool ControllableContainer::saveCurrentPreset()
{
	//Same as saveNewPreset because PresetManager now replaces if name is the same
	if (currentPreset == nullptr) {
		jassertfalse;
		return false;
	}

	PresetManager::Preset * pre = PresetManager::getInstance()->addPresetFromControllableContainer(currentPreset->name, getPresetFilter(), this, presetSavingIsRecursive);
	savePresetInternal(pre);
	NLOG(niceName, "Current preset saved : " + pre->name);
	loadPreset(pre);

	/*
  for (auto &pv : currentPreset->presetValues)
  {
	Parameter * p = dynamic_cast<Parameter*> (getControllableForAddress(pv->paramControlAddress));
	if (p != nullptr && p!=currentPresetName)
	{
	  pv->presetValue = var(p->value);
	}
  }
  savePresetInternal(currentPreset);
  NLOG(niceName, "Current preset saved : " + currentPreset->name);

  return true;
  */

	return true;
}

int ControllableContainer::getNumPresets()
{
	return PresetManager::getInstance()->getNumPresetForFilter(getPresetFilter());
}

bool ControllableContainer::resetFromPreset()
{
	if (currentPreset == nullptr) return false;


	for (auto &pv : currentPreset->presetValues)
	{
		Parameter * p = (Parameter *)getControllableForAddress(pv->paramControlAddress);
		if (p != nullptr && p != currentPresetName) p->resetValue();
	}


	currentPreset = nullptr;
	currentPresetName->setValue("", true);

	return true;
}

var ControllableContainer::getPresetValueFor(Parameter * p)
{
	if (currentPreset == nullptr) return var();
	return currentPreset->getPresetValue(p->getControlAddress(this));
}


String ControllableContainer::getPresetFilter()
{
	return shortName;
}


void ControllableContainer::dispatchFeedback(Controllable * c)
{
	//    @ben removed else here to enable containerlistener call back of non root (proxies) is it overkill?
	if (parentContainer != nullptr) { parentContainer->dispatchFeedback(c); }
	controllableContainerListeners.call(&ControllableContainerListener::controllableFeedbackUpdate, this, c);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableFeedbackUpdate, this, c));

}



void ControllableContainer::parameterValueChanged(Parameter * p)
{
	if (p->parentContainer == this)
	{
		onContainerParameterChanged(p);
		if (p->isControllableExposed) dispatchFeedback(p);
	} else
	{
		onExternalParameterChanged(p);
	}

}


void ControllableContainer::triggerTriggered(Trigger * t)
{
	if (t->parentContainer == this)
	{
		if (t == savePresetTrigger) saveCurrentPreset();
		else onContainerTriggerTriggered(t);
	} else
	{
		onExternalTriggerTriggered(t);
	}

	if (t->isControllableExposed) dispatchFeedback(t);
}

void ControllableContainer::askForRemoveControllable(Controllable * c)
{
	removeControllable(c);
}




var ControllableContainer::getJSONData()
{
	var data(new DynamicObject());

	var paramsData;

	Array<WeakReference<Parameter>> cont = ControllableContainer::getAllParameters(saveAndLoadRecursiveData, true);

	for (auto &wc : cont) {
		if (wc.wasObjectDeleted()) continue;
		if (!wc->isSavable) continue;
		if (wc == currentPresetName && !canHavePresets) continue;
		paramsData.append(wc->getJSONData(this));
	}

	/*
	 if (currentPreset != nullptr)
	 {
	 data.getDynamicObject()->setProperty(presetIdentifier, currentPreset->name);
	 }
	 */

	data.getDynamicObject()->setProperty("uid", uid.toString());
	data.getDynamicObject()->setProperty("parameters", paramsData);
	if (saveAndLoadName)
	{
		data.getDynamicObject()->setProperty("niceName", niceName);
		if (hasCustomShortName) data.getDynamicObject()->setProperty("shortName", shortName);
	}
	return data;
}

void ControllableContainer::loadJSONData(var data, bool createIfNotThere)
{
	if (data.isVoid()) return;

	if (data.getDynamicObject()->hasProperty("uid")) uid = data.getDynamicObject()->getProperty("uid");
	if (data.getDynamicObject()->hasProperty("niceName")) setNiceName(data.getDynamicObject()->getProperty("niceName"));
	if (data.getDynamicObject()->hasProperty("shortName")) setCustomShortName(data.getDynamicObject()->getProperty("shortName"));

	Array<var> * paramsData = data.getDynamicObject()->getProperty("parameters").getArray();

	if (paramsData != nullptr)
	{
		for (var &pData : *paramsData)
		{
			DynamicObject * o = pData.getDynamicObject();
			String pControlAddress = o->getProperty("controlAddress");

			Controllable * c = getControllableForAddress(pControlAddress, saveAndLoadRecursiveData, true);

			if (c != nullptr)
			{
				if (Parameter * p = dynamic_cast<Parameter*>(c)) {
					//                we don't load preset when already loading a state
					if (p->shortName != "preset" && p->isSavable) p->loadJSONData(pData.getDynamicObject());

				}
			} else if (!saveAndLoadRecursiveData && createIfNotThere)
			{
				c = ControllableFactory::getInstance()->createControllable(o->getProperty("type"));
				if (c != nullptr)
				{
					c->saveValueOnly = false; //auto set here because it will likely need that if it has been created from data
					c->loadJSONData(pData);
					addControllable(c);
				}
			}
		}
	}


	loadJSONDataInternal(data);
}

void ControllableContainer::childStructureChanged(ControllableContainer *)
{
	notifyStructureChanged();
}

String ControllableContainer::getUniqueNameInContainer(const String & sourceName, int suffix)
{
	String resultName = sourceName;
	if (suffix > 0)
	{
		StringArray sa;
		sa.addTokens(resultName, false);
		if (sa.size() > 1 && (sa[sa.size() - 1].getIntValue() != 0 || sa[sa.size() - 1].containsOnly("0")))
		{
			int num = sa[sa.size() - 1].getIntValue() + suffix;
			sa.remove(sa.size() - 1);
			sa.add(String(num));
			resultName = sa.joinIntoString(" ");
		} else
		{
			resultName += " " + String(suffix);
		}
	}

	if (getControllableByName(resultName, true) != nullptr)
	{
		return getUniqueNameInContainer(sourceName, suffix + 1);
	}

	if (getControllableContainerByName(resultName, true) != nullptr)
	{
		return getUniqueNameInContainer(sourceName, suffix + 1);
	}

	return resultName;
}

DynamicObject * ControllableContainer::createScriptObject()
{
	DynamicObject * o = ScriptTarget::createScriptObject();

	for (auto &cc : controllableContainers)
	{
		if (!cc->includeInScriptObject) continue;
		o->setProperty(cc->shortName, cc->createScriptObject());
	}

	for (auto &c : controllables)
	{
		o->setProperty(c->shortName, c->createScriptObject());
	}

	o->setProperty("name", shortName);
	o->setProperty("niceName", niceName);

	return o;
}

var ControllableContainer::getChildFromScript(const var::NativeFunctionArgs & a)
{
	if (a.numArguments == 0) return var();
	ControllableContainer * m = getObjectFromJS<ControllableContainer>(a);
	if (m == nullptr) return var();
	Controllable * c = m->getControllableByName(a.arguments[0].toString());
	if (c == nullptr) return  var();
	return c->createScriptObject();
}


InspectableEditor * ControllableContainer::getEditor(bool isRoot)
{
	return new GenericControllableContainerEditor(this, isRoot);
}
