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


const Identifier ControllableContainer::presetIdentifier("preset");
const Identifier ControllableContainer::paramIdentifier("parameters");


const Identifier ControllableContainer::controlAddressIdentifier("controlAddress");
const Identifier ControllableContainer::valueIdentifier("value");


ControllableContainer::ControllableContainer(const String & niceName) :
	parentContainer(nullptr),
	hasCustomShortName(false),
	skipControllableNameInAddress(false),
	currentPreset(nullptr),
	canHavePresets(true),
	saveAndLoadRecursiveData(true),
	numContainerIndexed(0),
	localIndexedPosition(-1),
	presetSavingIsRecursive(false)
{
  setNiceName(niceName);
  currentPresetName = addStringParameter("Preset", "Current Preset", "");
  currentPresetName->hideInEditor = true;
  savePresetTrigger = addTrigger("Save Preset", "Save current preset");
  savePresetTrigger->hideInEditor = true;

}

ControllableContainer::~ControllableContainer()
{
  //controllables.clear();
  //DBG("CLEAR CONTROLLABLE CONTAINER");

  clear();
  masterReference.clear();
}
void ControllableContainer::clear(){
  controllables.clear();
  controllableContainers.clear();
}

void ControllableContainer::addParameter(Parameter * p)
{
  addParameterInternal(p);
}

FloatParameter * ControllableContainer::addFloatParameter(const String & _niceName, const String & description, const float & initialValue, const float & minValue, const float & maxValue, const bool & enabled)
{
  String targetName = getUniqueNameInContainer(_niceName);
  FloatParameter * p = new FloatParameter(targetName, description, initialValue, minValue, maxValue, enabled);
  addParameterInternal(p);
  return p;
}

IntParameter * ControllableContainer::addIntParameter(const String & _niceName, const String & _description, const int & initialValue, const int & minValue, const int & maxValue, const bool & enabled)
{
  String targetName = getUniqueNameInContainer(_niceName);
  IntParameter * p = new IntParameter(targetName, _description, initialValue, minValue, maxValue, enabled);
  addParameterInternal(p);
  return p;
}

BoolParameter * ControllableContainer::addBoolParameter(const String & _niceName, const String & _description, const bool & value, const bool & enabled)
{
  String targetName = getUniqueNameInContainer(_niceName);
  BoolParameter * p = new BoolParameter(targetName, _description, value, enabled);
  addParameterInternal(p);
  return p;
}

StringParameter * ControllableContainer::addStringParameter(const String & _niceName, const String & _description, const String &value, const bool & enabled)
{
  String targetName = getUniqueNameInContainer(_niceName);
  StringParameter * p = new StringParameter(targetName, _description, value, enabled);
  addParameterInternal(p);
  return p;
}

EnumParameter * ControllableContainer::addEnumParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	EnumParameter * p = new EnumParameter(targetName, _description, enabled);
	addParameterInternal(p);
	return p;
}

Point2DParameter * ControllableContainer::addPoint2DParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	Point2DParameter * p = new Point2DParameter(targetName, _description, enabled);
	addParameterInternal(p);
	return p;
}

Point3DParameter * ControllableContainer::addPoint3DParameter(const String & _niceName, const String & _description, const bool & enabled)
{
	String targetName = getUniqueNameInContainer(_niceName);
	Point3DParameter * p = new Point3DParameter(targetName, _description, enabled);
	addParameterInternal(p);
	return p;
}

Trigger * ControllableContainer::addTrigger(const String & _niceName, const String & _description, const bool & enabled)
{
  String targetName = getUniqueNameInContainer(_niceName);
  Trigger * t = new Trigger(targetName, _description, enabled);
  controllables.add(t);
  t->setParentContainer(this);
  t->addTriggerListener(this);

  controllableContainerListeners.call(&ControllableContainerListener::controllableAdded, t);
  notifyStructureChanged();
  return t;
}

void ControllableContainer::removeControllable(Controllable * c)
{
  controllableContainerListeners.call(&ControllableContainerListener::controllableRemoved, c);

  if(Parameter * p = dynamic_cast<Parameter*>(c)){
    p->removeParameterListener(this);
    p->removeAsyncParameterListener(this);
  }
  controllables.removeObject(c);
  notifyStructureChanged();
}


void ControllableContainer::notifyStructureChanged(){

  controllableContainerListeners.call(&ControllableContainerListener::childStructureChanged, this);
}

void ControllableContainer::newMessage(const Parameter::ParamWithValue& pv){
  if(pv.parameter == currentPresetName){
    loadPresetWithName(pv.parameter->stringValue());
  }
  if(!pv.isRange()){
  onContainerParameterChangedAsync(pv.parameter, pv.value);
  }
}
void ControllableContainer::setNiceName(const String &_niceName) {
  if (niceName == _niceName) return;
  niceName = _niceName;
  if (!hasCustomShortName) setAutoShortName();
}

void ControllableContainer::setCustomShortName(const String &_shortName){
  shortName = _shortName;
  hasCustomShortName = true;
  updateChildrenControlAddress();
  controllableContainerListeners.call(&ControllableContainerListener::childAddressChanged,this);
}

void ControllableContainer::setAutoShortName() {
  hasCustomShortName = false;
  shortName = StringUtil::toShortName(niceName);
  updateChildrenControlAddress();
  controllableContainerListeners.call(&ControllableContainerListener::childAddressChanged,this);
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

  controllableContainers.add(container);
  container->addControllableContainerListener(this);
  container->setParentContainer(this);
  controllableContainerListeners.call(&ControllableContainerListener::controllableContainerAdded, container);
  notifyStructureChanged();
}

void ControllableContainer::removeChildControllableContainer(ControllableContainer * container)
{
  if(numContainerIndexed>0 &&
     container->localIndexedPosition>=0 &&
     controllableContainers.getUnchecked(container->localIndexedPosition) == container){
    numContainerIndexed--;
  }
  this->controllableContainers.removeAllInstancesOf(container);
  container->removeControllableContainerListener(this);
  controllableContainerListeners.call(&ControllableContainerListener::controllableContainerRemoved, container);
  notifyStructureChanged();
  container->setParentContainer(nullptr);
}

void ControllableContainer::addChildIndexedControllableContainer(ControllableContainer * container,int idx){
  if(idx == -1 )idx = numContainerIndexed;
  jassert(idx<=numContainerIndexed);

  controllableContainers.insert(idx, container);
  container->localIndexedPosition = idx;
  numContainerIndexed++;

  container->addControllableContainerListener(this);
  container->setParentContainer(this);
  controllableContainerListeners.call(&ControllableContainerListener::controllableContainerAdded, container);
  notifyStructureChanged();
}

void ControllableContainer::removeChildIndexedControllableContainer(int idx){
  if(idx == -1 )idx = numContainerIndexed-1;
  jassert(idx<numContainerIndexed);


  removeChildControllableContainer(controllableContainers.getUnchecked(idx));
  numContainerIndexed--;

  for(int i = idx ; i < numContainerIndexed ; i ++){
    controllableContainers.getUnchecked(i)->localIndexedPosition = i;
    controllableContainers.getUnchecked(i)->localIndexChanged();
  }

}

int ControllableContainer::getNumberOfIndexedContainer(){return numContainerIndexed;}
int ControllableContainer::getIndexedPosition(){return localIndexedPosition;}
bool ControllableContainer::hasIndexedContainers(){return numContainerIndexed>0;}
bool ControllableContainer::isIndexedContainer(){return localIndexedPosition>=0;}
void ControllableContainer::localIndexChanged(){};

ControllableContainer * ControllableContainer::getControllableContainerByName(const String & name, bool searchNiceNameToo)
{
  for (auto &cc : controllableContainers)
  {
    if (cc.get() && (cc->shortName == name || (searchNiceNameToo && cc->niceName == name))) return cc;
  }

  return nullptr;

}

ControllableContainer * ControllableContainer::getControllableContainerForAddress( StringArray  addressSplit)
{

  if (addressSplit.size() == 0) jassertfalse; // SHOULD NEVER BE THERE !

  bool isTargetAControllable = addressSplit.size() == 1;

  if (isTargetAControllable)
  {

    if(ControllableContainer * res = getControllableContainerByName(addressSplit[0]))
      return res;

    //no found in direct children Container, maybe in a skip container ?
    for (auto &cc : controllableContainers)
    {
      if (cc->skipControllableNameInAddress)
      {
        if (ControllableContainer * res = cc->getControllableContainerForAddress(addressSplit)) return res;
      }
    }
  }
  else
  {
    for (auto &cc : controllableContainers)
    {

      if (!cc->skipControllableNameInAddress)
      {
        if (cc->shortName == addressSplit[0])
        {
          addressSplit.remove(0);
          return cc->getControllableContainerForAddress(addressSplit);
        }
      }
      else
      {
        ControllableContainer * tc = cc->getControllableContainerByName(addressSplit[0]);
        if (tc != nullptr)
        {
          addressSplit.remove(0);
          return tc->getControllableContainerForAddress(addressSplit);
        }

      }
    }
  }

  return nullptr;

}

String ControllableContainer::getControlAddress(ControllableContainer * relativeTo){
  StringArray addressArray;
  ControllableContainer * pc = this;
  while (pc != relativeTo && pc != nullptr)
  {
    if(!pc->skipControllableNameInAddress) addressArray.insert(0, pc->shortName);
    pc = pc->parentContainer;
  }
  if(addressArray.size()==0)return "";
  else return "/" + addressArray.joinIntoString("/");
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

Array<Controllable*> ControllableContainer::getAllControllables(bool recursive,bool getNotExposed)
{
  Array<Controllable*> result;
  for (auto &c : controllables)
  {
    if (getNotExposed || c->isControllableExposed) result.add(c);
  }

  if (recursive)
  {
    for (auto &cc : controllableContainers) result.addArray(cc->getAllControllables(true,getNotExposed));
  }

  return result;
}

Array<Parameter*> ControllableContainer::getAllParameters(bool recursive, bool getNotExposed)
{
  Array<Parameter*> result;
  for (auto &c : controllables)
  {
    if (c->type == Controllable::Type::TRIGGER) continue;
      if (getNotExposed || c->isControllableExposed){
          if(Parameter * cc = dynamic_cast<Parameter*>(c)){
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



Controllable * ControllableContainer::getControllableForAddress(String address, bool recursive, bool getNotExposed)
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
      if (cc->skipControllableNameInAddress)
      {
        Controllable * tc = cc->getControllableByName(addressSplit[0]);

        if (tc != nullptr) return tc;
      }
    }
  }
  else
  {
    for (auto &cc : controllableContainers)
    {

      if (!cc->skipControllableNameInAddress)
      {
        if (cc->shortName == addressSplit[0])
        {
          addressSplit.remove(0);
          return cc->getControllableForAddress(addressSplit,recursive,getNotExposed);
        }
      }
      else
      {
        ControllableContainer * tc = cc->getControllableContainerByName(addressSplit[0]);
        if (tc != nullptr)
        {
          addressSplit.remove(0);
          return tc->getControllableForAddress(addressSplit,recursive,getNotExposed);
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
    if(isLoadingPreset){return false;}
    if(name=="") return false;
    isLoadingPreset = true;

	PresetManager::Preset * preset = PresetManager::getInstance()->getPreset(getPresetFilter(), name);
    if (preset == nullptr){isLoadingPreset = false;currentPresetName->setValue("", true); return false;}
	bool hasLoaded = loadPreset(preset);
    isLoadingPreset = false;
    return hasLoaded;

}

bool ControllableContainer::loadPreset(PresetManager::Preset * preset)
{
	if (preset == nullptr){
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
    if (p != nullptr && p !=currentPresetName) p->resetValue();
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
  if (parentContainer != nullptr){ parentContainer->dispatchFeedback(c); }
  controllableContainerListeners.call(&ControllableContainerListener::controllableFeedbackUpdate,this, c);

}



void ControllableContainer::parameterValueChanged(Parameter * p)
{
	onContainerParameterChanged(p);
	if (p->isControllableExposed) dispatchFeedback(p);
}


void ControllableContainer::triggerTriggered(Trigger * t)
{

	if (t == savePresetTrigger)
	{
		saveCurrentPreset();
		
	} else
	{
		onContainerTriggerTriggered(t);
	}

    if (t->isControllableExposed) dispatchFeedback(t);

	

}


void ControllableContainer::addParameterInternal(Parameter * p)
{
  p->setParentContainer(this);
  controllables.add(p);
  p->addParameterListener(this);
  p->addAsyncParameterListener(this);
  controllableContainerListeners.call(&ControllableContainerListener::controllableAdded, p);
  notifyStructureChanged();
}





var ControllableContainer::getJSONData()
{
  var data(new DynamicObject());

  var paramsData;


  Array<Controllable *> cont = ControllableContainer::getAllControllables(saveAndLoadRecursiveData, true);

  for (auto &c : cont) {
    Parameter * base = dynamic_cast<Parameter*>(c);
    if (base )
    {
      if(base->isSavable){
        var pData(new DynamicObject());
        pData.getDynamicObject()->setProperty(controlAddressIdentifier, base->getControlAddress(this));
        pData.getDynamicObject()->setProperty(valueIdentifier, base->value);
        paramsData.append(pData);
      }
    }
    else if (dynamic_cast<Trigger*>(c) != nullptr) {

    }
    else {
      // should never happen un less another Controllable type than parameter or trigger has been introduced
      jassertfalse;
    }
  }

  /*
   if (currentPreset != nullptr)
   {
   data.getDynamicObject()->setProperty(presetIdentifier, currentPreset->name);
   }
   */


  data.getDynamicObject()->setProperty("uid",uid.toString());
  data.getDynamicObject()->setProperty(paramIdentifier, paramsData);

  return data;
}

void ControllableContainer::loadJSONData(var data)
{

  if (data.getDynamicObject()->hasProperty("uid")) uid = data.getDynamicObject()->getProperty("uid");

  // @ ben we don't want to load preset when loading from file, do we?

  //    if (data.getDynamicObject()->hasProperty(presetIdentifier))
  //    {
  //        loadPresetWithName(data.getDynamicObject()->getProperty("preset"));
  //    }

  // TODO switch Array to dynamic object (to avoid to store control address and store parameters in a namedValueSet )
  Array<var> * paramsData = data.getDynamicObject()->getProperty(paramIdentifier).getArray();

  if (paramsData != nullptr)
  {
	  for (var &pData : *paramsData)
	  {
		  String pControlAddress = pData.getDynamicObject()->getProperty(controlAddressIdentifier);

		  Controllable * c = getControllableForAddress(pControlAddress, saveAndLoadRecursiveData, true);

		  if (Parameter * p = dynamic_cast<Parameter*>(c)) {
			  //                we don't load preset when already loading a state
			  if (p->shortName != presetIdentifier.toString() && p->isSavable) p->setValue(pData.getDynamicObject()->getProperty(valueIdentifier));

		  } else {
			  //NLOG("LoadJSON : "+niceName,"Parameter not found "+ pControlAddress);
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
    sa.addTokens(resultName,false);
    if (sa.size() > 1 && (sa[sa.size()-1].getIntValue() != 0 || sa[sa.size()-1].containsOnly("0")))
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

  if (getControllableByName(resultName,true) != nullptr)
  {
    return getUniqueNameInContainer(sourceName, suffix + 1);
  }

  if (getControllableContainerByName(resultName,true) != nullptr)
  {
    return getUniqueNameInContainer(sourceName, suffix + 1);
  }

  return resultName;
}
