/*
  ==============================================================================

    UserOSCCommandModel.h
    Created: 15 Jan 2017 4:53:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODEL_H_INCLUDED
#define USEROSCCOMMANDMODEL_H_INCLUDED


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
	void loadJSONDataInternal(var data) override;

	void onContainerNiceNameChanged() override;

	virtual String getTypeString() const override { return param->getTypeString(); }

};

class OSCCommandModelArgumentManager :
	public BaseManager<OSCCommandModelArgument>
{
public:
	OSCCommandModelArgumentManager();
	~OSCCommandModelArgumentManager() {}

	void addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false); 
	void addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);

	void addItemFromData(var data, bool fromUndoableAction = false) override;

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
