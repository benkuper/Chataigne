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

	autoRange = addBoolParameter("Auto Range", "If checked, will change range of already existing values if more than one argument is provided in the incoming OSC message", false);
	autoRange->isTargettable = false;

	splitArgs = addBoolParameter("Split Arguments", "If checked, a message with multiple arguments will be exploded in multliple values", false);
	splitArgs->isTargettable = false;


	defManager.add(CommandDefinition::createDef(this, "", "Custom Message", &CustomOSCCommand::create));
	addChildControllableContainer(&umm);
	umm.addBaseManagerListener(this);
}

void CustomOSCModule::processMessageInternal(const OSCMessage & msg)
{
	if (autoAdd == nullptr) return;

	//first we remove slashes to allow for simple controllableContainer search
	const String cNiceName = msg.getAddressPattern().toString();
	const String cShortName = cNiceName.replaceCharacters("/", "_");
	Controllable * c = nullptr;

	if(msg.size() > 1 && splitArgs->boolValue())
	{
		for (int i = 0; i < msg.size(); i++)
		{
			c = valuesCC.getControllableByName(cShortName+"_"+String(i));
			Parameter * p = (Parameter *)c;
			if (c != nullptr)
			{
				switch (c->type)
				{
				case Controllable::BOOL: p->setValue(getFloatArg(msg[i]) >= 1); break;
				case Controllable::FLOAT: p->setValue(getFloatArg(msg[i]));break;
				case Controllable::INT: p->setValue(getIntArg(msg[i])); break;
				case Controllable::STRING: p->setValue(getStringArg(msg[i])); break;
                    default:
                        break;
				}
			} else
			{
				String argIAddress = cNiceName + "_" + String(i);
				if (msg[i].isInt32()) c = valuesCC.addIntParameter(argIAddress, "", msg[i].getInt32(), msg[i].getInt32(), msg[i].getInt32());
				else if (msg[i].isFloat32()) c = valuesCC.addFloatParameter(argIAddress, "", msg[i].getFloat32());
				else if (msg[i].isString()) c = valuesCC.addStringParameter(argIAddress, "", msg[i].getString());
				((Parameter *)c)->autoAdaptRange = true;
			}
		}
	} else
	{
		c = valuesCC.getControllableByName(cShortName);
		if (c != nullptr) //update existing controllable
		{
			switch (c->type)
			{
			case Controllable::TRIGGER:
				((Trigger *)c)->trigger();
				break;

			case Controllable::BOOL: 
				((Parameter *)c)->setValue(getFloatArg(msg[0]) >= 1); break;
				break;

			case Controllable::FLOAT:
				if (msg.size() >= 1)
				{
					FloatParameter *f = (FloatParameter *)c;
					if (msg.size() >= 3 && autoRange->boolValue()) f->setRange(getFloatArg(msg[1]), getFloatArg(msg[2]));
					f->setValue(getFloatArg(msg[0]));
				}
				break;

			case Controllable::INT:
				if (msg.size() >= 1)
				{
					IntParameter *i = (IntParameter *)c;
					if (msg.size() >= 3 && autoRange->boolValue()) i->setRange(getIntArg(msg[1]), getIntArg(msg[2]));

					i->setValue(getIntArg(msg[0]));
				}
				break;

			case Controllable::STRING:
				if (msg.size() >= 1) ((StringParameter *)c)->setValue(getStringArg(msg[0]));
				break;

			case Controllable::POINT2D:
				if (msg.size() >= 2) ((Point2DParameter *)c)->setPoint(getFloatArg(msg[0]), getFloatArg(msg[1]));
				break;

			case Controllable::POINT3D:
				if (msg.size() >= 3) ((Point3DParameter *)c)->setVector(Vector3D<float>(getFloatArg(msg[0]), getFloatArg(msg[1]), getFloatArg(msg[2])));
				break;

			default:
				//not handled
				break;
			}
		}
	}



	//ADDING VALUE
	
	if(c == nullptr && autoAdd->boolValue()) //if auto add, add a new value
	{
		const int numArgs = msg.size();
		
		switch (numArgs)
		{
		case 0:
			c = new Trigger(cNiceName, "");
			break;

		case 1:
			if (msg[0].isInt32()) c = new IntParameter(cNiceName, "", msg[0].getInt32(), msg[0].getInt32(), msg[0].getInt32());
			else if (msg[0].isFloat32()) c = new FloatParameter(cNiceName, "", msg[0].getFloat32());
			else if (msg[0].isString()) c = new StringParameter(cNiceName, "", msg[0].getString());
			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32()) c = new IntParameter(cNiceName, "", getIntArg(msg[0]), getIntArg(msg[1]), getIntArg(msg[1]) + 1);
			else if (msg[0].isFloat32())
			{
				c = new Point2DParameter(cNiceName, "");
				((Point2DParameter *)c)->setPoint(getFloatArg(msg[0]), getFloatArg(msg[1]));
			} else if (msg[0].isString()) 
			{
				c = new StringParameter(cNiceName, "", getStringArg(msg[0]));
			}
			break;

		case 3:
			if (msg[0].isInt32()) c = new IntParameter(cNiceName, "", getIntArg(msg[0]), getIntArg(msg[1]), getIntArg(msg[2]));
			else if (msg[0].isFloat32())
			{
				c = new Point3DParameter(cNiceName, "");
				((Point3DParameter *)c)->setVector(getFloatArg(msg[0]), getFloatArg(msg[1]), getFloatArg(msg[2]));
			} else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", getStringArg(msg[0]));
			}
			break;

		default:
			//DBG("Message " + cNiceName + "num arguments not handled : " + numArgs);
			return;
		}

		
		c->setCustomShortName(cShortName); //force safeName for search
		c->isCustomizableByUser = true;
		c->isRemovableByUser = true;
		c->saveValueOnly = false;
		if(c->type != Controllable::TRIGGER) ((Parameter *)c)->autoAdaptRange = true;

		valuesCC.addControllable(c);
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
