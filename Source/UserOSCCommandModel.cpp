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
	BaseItem("New Model",false),
	argumentsContainer("arguments")
{
	addressParam = addStringParameter("OSC Address", "OSC Adress that will sent", "/example");
	addressIsEditable = addBoolParameter("Address is Editable", "If check, the address will be editable in each command created", false);
	addChildControllableContainer(&argumentsContainer);	
}

UserOSCCommandModel::~UserOSCCommandModel()
{
	argumentsContainer.clear();
}

void UserOSCCommandModel::addIntArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new IntParameter("#" + id, "Argument #" + id + ", type int", 0, -1000, 1000);
	addArgument(p);
}

void UserOSCCommandModel::addFloatArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new FloatParameter("#" + id, "Argument #" + id + ", type int", 0, 0, 1);
	addArgument(p);
}

void UserOSCCommandModel::addStringArgument()
{
	String id = String(argumentsContainer.controllables.size() + 1);
	Parameter * p = new StringParameter("#" + id, "Argument #" + id + ", type int", "myString");
	addArgument(p);
}

void UserOSCCommandModel::addArgument(Parameter * p)
{
	p->saveValueOnly = false;
	OSCCommandModelArgument * a = new OSCCommandModelArgument(p);
	arguments.add(a);
	argumentsContainer.addChildControllableContainer(a);
}

InspectableEditor * UserOSCCommandModel::getEditor(bool isRoot)
{
	return new UserOSCCommandModelEditor(this, isRoot);
}



//MODEL ARGUMENT

OSCCommandModelArgument::OSCCommandModelArgument(Parameter * _p) :
	ControllableContainer("arg"),
	param(_p)
{
	addControllable(param);
	argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
	editable = addBoolParameter("Editable", "If uncheck, argument will always be used at its default value, and not visible in the command editor", true);
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
	def->setup(container, "", def->model->niceName, createFunc);
	def->addParam("model", def->model->shortName);
	return def;
}
