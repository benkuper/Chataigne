/*
  ==============================================================================

    UserOSCCommand.cpp
    Created: 15 Jan 2017 4:43:14pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommand.h"
#include "UserOSCCommandEditor.h"

UserOSCCommand::UserOSCCommand(CustomOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	cModule(_module)
{
	model = cModule->getModelForName(params.getProperty("model", ""));
	jassert(model != nullptr);
	
	address->setValue(model->addressParam->stringValue());
	address->isEditable = model->addressIsEditable->boolValue();
	rebuildArgsFromModel();
}

UserOSCCommand::~UserOSCCommand()
{
}

void UserOSCCommand::rebuildArgsFromModel()
{
	argumentsContainer.clear();

	//TODO : SlaveParameter ?
	Parameter * p = nullptr;
	for (auto &a : model->arguments.items)
	{
		Parameter * ap = a->param;
		switch (ap->type)
		{
		case Controllable::BOOL: p = new BoolParameter(a->niceName, ap->description, ap->value); break;
		case Controllable::INT: p = new IntParameter(a->niceName, ap->description, ap->value,ap->minimumValue,ap->maximumValue); break;
		case Controllable::FLOAT: p = new FloatParameter(a->niceName, ap->description, ap->value, ap->minimumValue, ap->maximumValue); break;
		case Controllable::STRING: p = new StringParameter(a->niceName, ap->description, ap->value); break;
		}
		
		argumentsContainer.addParameter(p);
	}

	argumentsContainer.hideInEditor = model->arguments.items.size() == 0;
}

InspectableEditor * UserOSCCommand::getEditor(bool isRoot)
{
	return new UserOSCCommandEditor(this, isRoot);
}
