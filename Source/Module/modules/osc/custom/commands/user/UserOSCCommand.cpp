/*
  ==============================================================================

	UserOSCCommand.cpp
	Created: 15 Jan 2017 4:43:14pm
	Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommand.h"
#include "ui/UserOSCCommandEditor.h"

UserOSCCommand::UserOSCCommand(CustomOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	cModule(_module)
{
	model = cModule->getModelForName(params.getProperty("model", ""));
	jassert(model != nullptr);

	address->setValue(model->addressParam->stringValue());
	address->isEditable = model->addressIsEditable->boolValue();
	address->isSavable = address->isEditable;

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
	int mappingTargetIndex = 0;
	for (auto &a : model->arguments.items)
	{
		Parameter * ap = a->param;
		switch (ap->type)
		{
		case Controllable::BOOL: p = new BoolParameter(a->niceName, ap->description, ap->getValue()); break;
		case Controllable::INT: p = new IntParameter(a->niceName, ap->description, ap->getValue(), ap->minimumValue, ap->maximumValue); break;
		case Controllable::FLOAT: p = new FloatParameter(a->niceName, ap->description, ap->getValue(), ap->minimumValue, ap->maximumValue); break;
		case Controllable::STRING: p = new StringParameter(a->niceName, ap->description, ap->getValue()); break;
            default:
                break;
		}

		argumentsContainer.addParameter(p);
		p->isEditable = a->editable->boolValue();

		if (context == MAPPING)
		{
			//p->isEditable = a->editable->boolValue() && !a->useForMapping->boolValue();

			if (a->useForMapping->boolValue())
			{
				setTargetMappingParameterAt(p, mappingTargetIndex);
				mappingTargetIndex++;
			}
		}
		
	}

	argumentsContainer.hideInEditor = model->arguments.items.size() == 0;
}