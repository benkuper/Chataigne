/*
  ==============================================================================

    CustomOSCInput.cpp
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCInput.h"

CustomOSCInput::CustomOSCInput() :
	OSCInput("OSC")
{

	valuesCC.saveAndLoadRecursiveData = false;

	autoAdd = addBoolParameter("Auto Add", "Add automatically any message that is received\nand try to create the corresponding value depending on the message content.", true);
	autoAdd->isTargettable = false;

}

void CustomOSCInput::processMessageInternal(const OSCMessage & msg)
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
				IntParameter *f = (IntParameter *)c;
				if (msg.size() >= 3) f->setRange(getIntArg(msg[1]), getIntArg(msg[2]));
				f->setValue(getIntArg(msg[0]));
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
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", msg[0].getInt32(), INT_MIN, INT_MAX);
			else if (msg[0].isFloat32()) c = valuesCC.addFloatParameter(cNiceName, "", msg[0].getFloat32());
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", msg[0].getString());
			((Parameter *)c)->autoAdaptRange = true;
			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", msg[0].getInt32(), INT_MIN, INT_MAX);
			else if (msg[0].isFloat32())
			{
				c = valuesCC.addPoint2DParameter(cNiceName, "");
				((Point2DParameter *)c)->setPoint(msg[0].getFloat32(), msg[1].getFloat32());
			}
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", msg[0].getString());
			((Parameter *)c)->autoAdaptRange = true;
			break;

		case 3:
			if (msg[0].isInt32()) c = valuesCC.addIntParameter(cNiceName, "", msg[0].getInt32(), msg[1].getInt32(),msg[2].getInt32());
			else if (msg[0].isFloat32())
			{
				c = valuesCC.addPoint3DParameter(cNiceName, "");
				((Point3DParameter *)c)->setVector(msg[0].getFloat32(), msg[1].getFloat32(),msg[1].getFloat32());
			}
			else if (msg[0].isString()) c = valuesCC.addStringParameter(cNiceName, "", msg[0].getString());
			((Parameter *)c)->autoAdaptRange = true;
			break;
		}

		c->saveValueOnly = false;
		c->setCustomShortName(cShortName); //force safeName for search
		valuesCC.orderControllablesAlphabetically();
	}
}

var CustomOSCInput::getJSONData()
{
	var data = OSCInput::getJSONData();
	var itemsData = var();
	data.getDynamicObject()->setProperty("values", valuesCC.getJSONData());
	return data;
}

void CustomOSCInput::loadJSONDataInternal(var data)
{
	OSCInput::loadJSONDataInternal(data);
	valuesCC.loadJSONData(data.getProperty("values", var()),true);
	valuesCC.orderControllablesAlphabetically();
}
