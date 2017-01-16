/*
  ==============================================================================

    UserOSCCommandModel.h
    Created: 15 Jan 2017 4:53:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODEL_H_INCLUDED
#define USEROSCCOMMANDMODEL_H_INCLUDED

#include "BaseItem.h"
#include "CommandDefinition.h"
#include "ControllableContainer.h"

class UserOSCCommandDefinition;

class OSCCommandModelArgument :
	public ControllableContainer
{
public:
	OSCCommandModelArgument(Parameter * p);
	Parameter * param;
	StringParameter * argumentName; 
	BoolParameter * useForMapping;
	BoolParameter * editable;

};

class UserOSCCommandModel :
	public BaseItem
{
public:
	UserOSCCommandModel();
	~UserOSCCommandModel();

	StringParameter * addressParam;
	BoolParameter * addressIsEditable;

	ControllableContainer argumentsContainer;
	OwnedArray<OSCCommandModelArgument> arguments;

	void addIntArgument();
	void addFloatArgument();
	void addStringArgument();
	void addArgument(Parameter *);

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
