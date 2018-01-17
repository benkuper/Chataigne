/*
  ==============================================================================

    UserOSCCommandModel.cpp
    Created: 15 Jan 2017 4:53:09pm
    Author:  Ben

  ==============================================================================
*/


#include "UserOSCCommandModel.h"
#include "UserOSCCommandModelEditor.h"
#include "UserOSCCommand.h"


UserOSCCommandModel::UserOSCCommandModel() :
	BaseItem("New Model",false)
{
	addressParam = addStringParameter("OSC Address", "OSC Adress that will sent", "/example");
	addressIsEditable = addBoolParameter("Editable", "If check, the address will be editable in each command created", false);
	addChildControllableContainer(&arguments);
}

UserOSCCommandModel::~UserOSCCommandModel()
{
}

var UserOSCCommandModel::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("arguments", arguments.getJSONData());
	return data;
}

void UserOSCCommandModel::loadJSONDataInternal(var data)
{
	DBG("Load jsondata internal " << niceName);
	arguments.loadJSONData(data.getProperty("arguments", var()));
}



//MODEL ARGUMENT

OSCCommandModelArgument::OSCCommandModelArgument(const String &name, Parameter * _p) :
	BaseItem(name,false),
	param(_p)
{
	editorCanBeCollapsed = false;
	

	isSelectable = false;
	param->isCustomizableByUser = true;
	param->saveValueOnly = false;

	jassert(param != nullptr);
	addControllable(param);

	//argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
	useForMapping->hideInEditor = true;
	editable = addBoolParameter("Editable", "If uncheck, argument will always be used at its default value, and not visible in the command editor", true);
	editable->hideInEditor = true;
}

InspectableEditor * OSCCommandModelArgument::getEditor(bool isRoot)
{
	return new OSCCommandModelArgumentEditor(this, isRoot);
}

var OSCCommandModelArgument::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("param", param->getJSONData());
	return data;
}

void OSCCommandModelArgument::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	param->loadJSONData(data.getProperty("param", var()));
}

void OSCCommandModelArgument::onContainerNiceNameChanged()
{
	BaseItem::onContainerNiceNameChanged();
	param->setNiceName(niceName);
}

UserOSCCommandDefinition::UserOSCCommandDefinition(UserOSCCommandModel * _model) :
	model(_model)
{
	
	model->addControllableContainerListener(this);
}

UserOSCCommandDefinition::~UserOSCCommandDefinition()
{
	if(model != nullptr) model->removeControllableContainerListener(this);
}

void UserOSCCommandDefinition::childAddressChanged(ControllableContainer * cc)
{
	if (cc == model)
	{
		commandType = cc->niceName;
		//CHANGE PARAM TO SHORT NAME
		params.getDynamicObject()->setProperty("model", model->shortName);
	} else
	{
		DBG("Child address changed but not model");
	}
}

UserOSCCommandDefinition * UserOSCCommandDefinition::createDef(ControllableContainer * container, UserOSCCommandModel * _model,CreateCommandFunc createFunc)
{
	UserOSCCommandDefinition *def = new UserOSCCommandDefinition(_model);
	def->setup(container, "", def->model->niceName, CommandContext::BOTH, createFunc);
	def->addParam("model", def->model->shortName);
	return def;
}


//MANAGER

OSCCommandModelArgumentManager::OSCCommandModelArgumentManager() : 
	BaseManager("arguments") 
{
	selectItemWhenCreated = false;
}

OSCCommandModelArgument * OSCCommandModelArgumentManager::addItemWithParam(Parameter * p, var data, bool fromUndoableAction)
{
	OSCCommandModelArgument * a = new OSCCommandModelArgument("#"+String(items.size()+1),p);
	return addItem(a,data, fromUndoableAction);
}

OSCCommandModelArgument * OSCCommandModelArgumentManager::addItemFromType(Parameter::Type type, var data, bool fromUndoableAction)
{ 
	Parameter * p = nullptr;
	String id = String(items.size()+1);
	
	switch (type)
	{
	case Parameter::STRING:
		p = new StringParameter("#" + id, "Argument #" + id + ", type int", "myString");
		break;
	case Parameter::FLOAT:
		p = new FloatParameter("#" + id, "Argument #" + id + ", type foat", 0, 0, 1);
		break;
	case Parameter::INT:
		p = new IntParameter("#" + id, "Argument #" + id + ", type int", 0, -1000, 1000);
		break;
	case Parameter::BOOL:
		p = new BoolParameter("#" + id, "Argument #" + id + ", type bool", false);
		break;
        default:
            break;
	}
	
	if (p == nullptr) return nullptr;
	return addItemWithParam(p,data, fromUndoableAction);
}

OSCCommandModelArgument * OSCCommandModelArgumentManager::addItemFromData(var data, bool fromUndoableAction)
{
	String s = data.getProperty("type", "");
	if (s.isEmpty()) return nullptr;
	Parameter * p = (Parameter *)ControllableFactory::createControllable(s);
	if (p == nullptr)
	{
		LOG("Problem loading OSC Command model argument");
		return nullptr;
	}

	return addItemWithParam(p, data, fromUndoableAction);
}

void OSCCommandModelArgumentManager::autoRenameItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i]->niceName.startsWithChar('#')) items[i]->setNiceName("#" + String(i+1));
	}
}

void OSCCommandModelArgumentManager::removeItemInternal(OSCCommandModelArgument *)
{
	autoRenameItems();
}

InspectableEditor * OSCCommandModelArgumentManager::getEditor(bool isRoot)
{
	return new UserOSCCommandModelArgumentManagerEditor(this,isRoot);
}
