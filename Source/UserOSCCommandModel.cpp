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

InspectableEditor * UserOSCCommandModel::getEditor(bool isRoot)
{
	return new UserOSCCommandModelEditor(this, isRoot);
}



//MODEL ARGUMENT

OSCCommandModelArgument::OSCCommandModelArgument(const String &name, Parameter * _p) :
	BaseItem(name),
	param(_p)
{
	isSelectable = false;
	canBeDisabled = false;
	
	jassert(param != nullptr);
	addControllable(param);

	//argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
	editable = addBoolParameter("Editable", "If uncheck, argument will always be used at its default value, and not visible in the command editor", true);
}

InspectableEditor * OSCCommandModelArgument::getEditor(bool isRoot)
{
	return new OSCCommandModelArgumentEditor(this, isRoot);
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

void OSCCommandModelArgumentManager::addItemWithParam(Parameter * p)
{
	OSCCommandModelArgument * a = new OSCCommandModelArgument("#"+String(items.size()+1),p);
	addItem(a);
}

void OSCCommandModelArgumentManager::addItemFromType(Parameter::Type type)
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
	}
	
	jassert(p != nullptr);
	addItemWithParam(p);
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
