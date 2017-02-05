/*
  ==============================================================================

    CustomOSCModule.cpp
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCModule.h"
#include "CommandFactory.h"
#include "CustomOSCCommand.h"
#include "UserOSCCommand.h"
#include "CustomOSCModuleEditor.h"


CustomOSCModule::CustomOSCModule() :
	OSCModule("OSC"),
	autoAdd(nullptr)
{
	autoAdd = addBoolParameter("Auto Add", "Add automatically any message that is received\nand try to create the corresponding value depending on the message content.", true);
	autoAdd->isTargettable = false;

	defManager.add(CommandDefinition::createDef(this, "", "Custom Message", &CustomOSCCommand::create));
	addChildControllableContainer(&umm);
	umm.addBaseManagerListener(this);
}

void CustomOSCModule::processMessageInternal(const OSCMessage & msg)
{
	//first we remove slashes to allow for simple controllableContainer search
	const String cNiceName = msg.getAddressPattern().toString();
	const String cShortName = cNiceName.replaceCharacters("/", "_");
	Controllable * c = valuesCC.getControllableByName(cShortName);

	if (c != nullptr) //update existing controllable
	{
		switch (c->type)
		{
		case Controllable::TRIGGER:
			((Trigger *)c)->trigger();
			break;

		case Controllable::FLOAT:
			if (msg.size() >= 1)
			{
				FloatParameter *f = (FloatParameter *)c;
				if (msg.size() >= 3) f->setRange(getFloatArg(msg[1]), getFloatArg(msg[2]));
				f->setValue(getFloatArg(msg[0]));
			}
			break;

		case Controllable::INT:
			if (msg.size() >= 1)
			{
				IntParameter *i = (IntParameter *)c;
				if (msg.size() >= 3) i->setRange(getIntArg(msg[1]), getIntArg(msg[2]));

				i->setValue(getIntArg(msg[0]));
			}
			break;

		case Controllable::STRING:
			if (msg.size() >= 1) ((StringParameter *)c)->setValue(getStringArg(msg[0]));
			break;

		case Controllable::POINT2D:
			if (msg.size() >= 2) ((Point2DParameter *)c)->setPoint(getFloatArg(msg[0]),getFloatArg(msg[1]));
			break;

		case Controllable::POINT3D:
			if (msg.size() >= 3) ((Point3DParameter *)c)->setVector(Vector3D<float>(getFloatArg(msg[0]), getFloatArg(msg[1]),getFloatArg(msg[2])));
			break;

		default:
			//not handled
			break;
		}

	} else if(autoAdd->boolValue()) //if auto add, add a new value
	{
		const int numArgs = msg.size();
		switch (numArgs)
		{
		case 0:
			c = valuesCC.addTrigger(cNiceName, "");
			break;

		case 1:
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", msg[0].getInt32(), msg[0].getInt32(), msg[0].getInt32());
			else if (msg[0].isFloat32()) c = valuesCC.addFloatParameter(cNiceName, "", msg[0].getFloat32());
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", msg[0].getString());
			((Parameter *)c)->autoAdaptRange = true;
			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", getIntArg(msg[0]),getIntArg(msg[1]),getIntArg(msg[2]));
			else if (msg[0].isFloat32())
			{
				c = valuesCC.addPoint2DParameter(cNiceName, "");
				((Point2DParameter *)c)->setPoint(getFloatArg(msg[0]),getFloatArg(msg[1]));
			}
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", getStringArg(msg[0]));
			((Parameter *)c)->autoAdaptRange = true;
			break;

		case 3:
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", getIntArg(msg[0]),getIntArg(msg[1]),getIntArg(msg[2]));
			else if (msg[0].isFloat32())
			{
				c = valuesCC.addPoint3DParameter(cNiceName, "");
				((Point3DParameter *)c)->setVector(getFloatArg(msg[0]), getFloatArg(msg[1]), getFloatArg(msg[2]));
			}
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", getStringArg(msg[0]));
			((Parameter *)c)->autoAdaptRange = true;
			break;
		}

		c->setCustomShortName(cShortName); //force safeName for search
		c->isCustomizableByUser = true;
		c->isRemovableByUser = true;
		c->saveValueOnly = false;
		valuesCC.orderControllablesAlphabetically();
	}
}


var CustomOSCModule::getJSONData()
{
	var data = OSCModule::getJSONData();
	data.getDynamicObject()->setProperty("values", valuesCC.getJSONData());
	data.getDynamicObject()->setProperty("models", umm.getJSONData());
	return data;
}

void CustomOSCModule::loadJSONDataInternal(var data)
{
	OSCModule::loadJSONDataInternal(data);
	valuesCC.loadJSONData(data.getProperty("values", var()), true);
	valuesCC.orderControllablesAlphabetically();
	umm.loadJSONData(data.getProperty("models", var()),true);

}

InspectableEditor * CustomOSCModule::getEditor(bool isRoot)
{
	return new CustomOSCModuleEditor(this,isRoot);
}

void CustomOSCModule::itemAdded(UserOSCCommandModel * model)
{
	defManager.add(UserOSCCommandDefinition::createDef(this, model, &UserOSCCommand::create));
}

void CustomOSCModule::itemRemoved(UserOSCCommandModel * model)
{
	CommandDefinition * d = getDefinitionForModel(model);
	if (d == nullptr) return;
	defManager.remove(d);
}

CommandDefinition * CustomOSCModule::getDefinitionForModel(UserOSCCommandModel * model)
{
	for (auto &d : defManager.definitions)
	{
		if (d->commandType == model->niceName) return d; 
	}
	return nullptr;
}

UserOSCCommandModel * CustomOSCModule::getModelForName(const String &modelName)
{
	for (auto &m : umm.items)
	{
		if (m->shortName == modelName) return m;
	}
	return nullptr;
}