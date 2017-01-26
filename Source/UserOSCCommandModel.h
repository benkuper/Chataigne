/*
  ==============================================================================

    UserOSCCommandModel.h
    Created: 15 Jan 2017 4:53:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODEL_H_INCLUDED
#define USEROSCCOMMANDMODEL_H_INCLUDED

#include "BaseManager.h"
#include "CommandDefinition.h"

class UserOSCCommandDefinition;

class OSCCommandModelArgument :
	public BaseItem
{
public:
	OSCCommandModelArgument(const String &name = "arg", Parameter * p = nullptr);
	Parameter * param;
	//StringParameter * argumentName; 
	BoolParameter * useForMapping;
	BoolParameter * editable;

	InspectableEditor * getEditor(bool isRoot) override;
	
	var getJSONData() override;

	void onContainerNiceNameChanged() override;
};

class OSCCommandModelArgumentManager :
	public BaseManager<OSCCommandModelArgument>
{
public:
	OSCCommandModelArgumentManager();
	~OSCCommandModelArgumentManager() {}

	void addItemWithParam(Parameter * p, var data = var());
	void addItemFromType(Parameter::Type type, var data = var());

	void addItemFromData(var data) override;

	void autoRenameItems();
	void removeItemInternal(OSCCommandModelArgument * i) override;
	InspectableEditor * getEditor(bool isRoot) override;
	
};

class UserOSCCommandModel :
	public BaseItem
{
public:
	UserOSCCommandModel();
	~UserOSCCommandModel();

	StringParameter * addressParam;
	BoolParameter * addressIsEditable;

	OSCCommandModelArgumentManager arguments;

	
	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	InspectableEditor * getEditor(bool /*isRoot*/) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModel)
};

class UserOSCCommandDefinition :
	public CommandDefinition,
	public ControllableContainerListener
{
public:
	UserOSCCommandDefinition(UserOSCCommandModel * model);
	~UserOSCCommandDefinition();

	UserOSCCommandModel * model;
	

	void childAddressChanged(ControllableContainer * cc) override;

	static UserOSCCommandDefinition * createDef(ControllableContainer * container, UserOSCCommandModel * _model, CreateCommandFunc createFunc);

};

#endif  // USEROSCCOMMANDMODEL_H_INCLUDED
