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
	modelRef = model;

	address->setValue(model->addressParam->stringValue());
	address->isEditable = model->addressIsEditable->boolValue();
	address->isSavable = address->isEditable;

	rebuildArgsFromModel();

	model->addCommandModelListener(this);
}

UserOSCCommand::~UserOSCCommand()
{
	if (!modelRef.wasObjectDeleted()) model->removeCommandModelListener(this);
}

void UserOSCCommand::rebuildArgsFromModel()
{
	var data = getJSONData();

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
		case Controllable::COLOR: p = new ColorParameter(a->niceName, ap->description, ((ColorParameter *)ap)->getColor()); break;
		case Controllable::POINT2D:
		{
			Point2DParameter * ap2D = dynamic_cast<Point2DParameter *>(ap);
			Point2DParameter * p2D = new Point2DParameter(a->niceName, ap->description);
			p2D->setBounds(ap2D->minimumValue[0], ap2D->minimumValue[1], ap2D->maximumValue[0], ap2D->maximumValue[1]);
			p2D->setPoint(ap2D->getPoint());
			p = p2D;
		}
		break;

		case Controllable::POINT3D: 
		{
			Point3DParameter * ap3D = dynamic_cast<Point3DParameter *>(ap);
			Point3DParameter * p3D = new Point3DParameter(a->niceName, ap->description);
			p3D->setBounds(ap3D->minimumValue[0], ap3D->minimumValue[1], ap3D->minimumValue[2], ap->maximumValue[0],ap3D->maximumValue[1],ap3D->maximumValue[2]);
			p3D->setVector(ap3D->getVector());
			p = p3D;
		}
		break;

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

	loadJSONData(data);
}

void UserOSCCommand::commandModelAddressChanged(UserOSCCommandModel *)
{
	address->defaultValue = model->addressParam->stringValue();

	if (!address->isOverriden) address->resetValue();

	address->isEditable = model->addressIsEditable->boolValue();
	address->isSavable = address->isEditable;
}

void UserOSCCommand::commandModelArgumentsChanged(UserOSCCommandModel *)
{
	rebuildArgsFromModel();
}

