/*
  ==============================================================================

	StreamingModule.cpp
	Created: 5 Jan 2018 10:39:38am
	Author:  Ben

  ==============================================================================
*/

#include "StreamingModule.h"
#include "commands/SendStreamRawDataCommand.h"
#include "commands/SendStreamStringCommand.h"
#include "commands/SendStreamValuesCommand.h"
#include "commands/SendStreamStringValuesCommand.h"
#include "UI/ChataigneAssetManager.h"

StreamingModule::StreamingModule(const String & name) :
	Module(name)
{
	includeValuesInSave = true;
	setupIOConfiguration(true, true);
	
	streamingType = moduleParams.addEnumParameter("Protocol", "Protocol for treating the incoming data");
	streamingType->addOption("Lines", LINES)->addOption("Raw", RAW)->addOption("Data255", DATA255)->addOption("COBS", COBS);

	autoAdd = moduleParams.addBoolParameter("Auto Add", "If checked, incoming data will be parsed depending on the Message Structure parameter, and if eligible will be added as values", true);
	messageStructure = moduleParams.addEnumParameter("Message Structure", "The expected structure of a message, determining how it should be interpreted to auto create values from it");
	firstValueIsTheName = moduleParams.addBoolParameter("First value is the name", "If checked, the first value of a parsed message will be used to name the value, otherwise each values will be named by their index", true);
	buildMessageStructureOptions();

	defManager.add(CommandDefinition::createDef(this, "", "Send string", &SendStreamStringCommand::create, CommandContext::BOTH));
	defManager.add(CommandDefinition::createDef(this, "", "Send values as string", &SendStreamStringValuesCommand::create, CommandContext::BOTH));
	defManager.add(CommandDefinition::createDef(this, "", "Send raw bytes", &SendStreamRawDataCommand::create, CommandContext::BOTH));
	defManager.add(CommandDefinition::createDef(this, "", "Send custom values", &SendStreamValuesCommand::create, CommandContext::BOTH)); 
	
	scriptObject.setMethod(sendId, StreamingModule::sendStringFromScript);
	scriptObject.setMethod(sendBytesId, StreamingModule::sendBytesFromScript);
	
	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("streaming");

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &StreamingModule::showMenuAndCreateValue;
}

StreamingModule::~StreamingModule()
{

}

void StreamingModule::buildMessageStructureOptions()
{
	StreamingType t = streamingType->getValueDataAsEnum<StreamingType>();
	messageStructure->clearOptions();

	switch (t)
	{
	
	case LINES:
	{
		messageStructure->addOption("Space separated", LINES_SPACE)->addOption("Tab separated", LINES_TAB)->addOption("Comma separated", LINES_COMMA);
	}
	break;
	
	case RAW:
	case COBS:
	case DATA255:
	{
		messageStructure->addOption("1 value per byte", RAW_1BYTE)->addOption("4x4 (floats)", RAW_FLOATS)->addOption("4x4 (RGBA colors)", RAW_COLORS);
	}
	break;
	}
}

void StreamingModule::processDataLine(const String & message)
{
	if (!enabled->boolValue()) return;
	if (logIncomingData->boolValue()) NLOG(niceName, "Message received : " + message);
	inActivityTrigger->trigger();

	scriptManager->callFunctionOnAllItems(dataEventId, message);


	MessageStructure s = messageStructure->getValueDataAsEnum<MessageStructure>();
	StringArray valuesString;
	String separator;
	switch (s)
	{
	case LINES_SPACE: separator = " "; break;
	case LINES_TAB:   separator = "\t"; break;
	case LINES_COMMA: separator = ","; break;
    default:
        break;
	}
	valuesString.addTokens(message, separator, "\"");
	if (valuesString.size() == 0)
	{
		//LOG("No usable data");
		return;
	}

	if (firstValueIsTheName->boolValue())
	{
		String valueName = valuesString[0];
		int numArgs = valuesString.size() - 1;
		Controllable * c = valuesCC.getControllableByName(valueName, true);
		if (c == nullptr)
		{
			if (!autoAdd->boolValue()) return;

			switch (numArgs)
			{
			case 0: c = new Trigger(valueName, valueName); break;
			case 1:	c = new FloatParameter(valueName, valueName, valuesString[1].getFloatValue()); break;
			case 2: c = new Point2DParameter(valueName, valueName); ((Point2DParameter *)c)->setPoint(valuesString[1].getFloatValue(), valuesString[2].getFloatValue()); break;
			case 3: c = new Point3DParameter(valueName, valueName); ((Point3DParameter *)c)->setVector(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue()); break;
			case 4: c = new ColorParameter(valueName, valueName, Colour::fromFloatRGBA(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue(), valuesString[4].getFloatValue()));
			}

			if (c != nullptr)
			{
				c->isCustomizableByUser = true;
				c->isRemovableByUser = true;
				c->saveValueOnly = false;
				valuesCC.addControllable(c);
			}
		} else
		{
			Controllable::Type t = c->type;
			switch (t)
			{
			case Controllable::TRIGGER:
				((Trigger *)c)->trigger();
				break;

			case Controllable::FLOAT:
				if (numArgs >= 1) ((FloatParameter *)c)->setValue(valuesString[1].getFloatValue());
				break;

			case Controllable::POINT2D:
				if (numArgs >= 2) ((Point2DParameter *)c)->setPoint(valuesString[1].getFloatValue(), valuesString[2].getFloatValue());
				break;

			case Controllable::POINT3D:
				if (numArgs >= 3) ((Point3DParameter *)c)->setVector(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue());
				break; 

			case Controllable::COLOR:
				if (numArgs >= 4) ((ColorParameter *)c)->setColor(Colour::fromFloatRGBA(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue(), valuesString[4].getFloatValue()));
				break;
                    
			case Controllable::STRING:
				if(numArgs >= 1) ((StringParameter *)c)->setValue(valuesString[1]);
				break;

            default:
                break;

			}
		}
		
	} else
	{
		
		int numArgs = valuesString.size();
		int numValues = valuesCC.controllables.size();

		if (autoAdd->boolValue())
		{
			while (numValues < numArgs)
			{
				FloatParameter * fp = new FloatParameter("Value " + String(numValues), "Value " + String(numValues), 0);
				fp->isCustomizableByUser = true;
				fp->isRemovableByUser = true;
				fp->saveValueOnly = false;

				valuesCC.addControllable(fp);

				numValues = valuesCC.controllables.size();
			}
		}

		for (int i = 0; i < numArgs; i++)
		{
			FloatParameter * c = dynamic_cast<FloatParameter *>(valuesCC.controllables[i]);
			if (c != nullptr)
			{
				c->setValue(valuesString[i].getFloatValue());
			}
		}

	}

}

void StreamingModule::processDataBytes(Array<uint8_t> data)
{
	if (!enabled->boolValue()) return;
	if (logIncomingData->boolValue())
	{
		String msg = String(data.size()) + "bytes received :";
		for (auto &d : data) msg += "\n" + String(d);
		NLOG(niceName, msg);
	}

	inActivityTrigger->trigger();

	if (scriptManager->items.size() > 0)
	{
		var args;
		for (auto &d : data) args.append(d);
		scriptManager->callFunctionOnAllItems(dataEventId, args);
	}


	MessageStructure st = messageStructure->getValueDataAsEnum<MessageStructure>();

	switch (st)
	{
	case RAW_1BYTE:
	{
		int numArgs = data.size();
		if (autoAdd->boolValue())
		{
			int numValues = valuesCC.controllables.size();
			while (numValues < numArgs)
			{
				IntParameter * p = new IntParameter("Value " + String(numValues), "Value " + String(numValues), 0, 0, 255);
				p->isCustomizableByUser = true;
				p->isRemovableByUser = true;
				p->saveValueOnly = false;
				valuesCC.addControllable(p);
				numValues = valuesCC.controllables.size();
			}
		}

		for (int i = 0; i < numArgs; i++)
		{
			IntParameter * c = dynamic_cast<IntParameter *>(valuesCC.controllables[i]);
			if (c != nullptr)
			{
				c->setValue(data[i]);
			}
		}
	}
	break;

	case RAW_FLOATS:
	{
		int numArgs = data.size() / 4;
		
		if (autoAdd->boolValue())
		{
			int numValues = valuesCC.controllables.size();
			while (numValues < numArgs)
			{
				FloatParameter * p = new FloatParameter("Value " + String(numValues), "Value " + String(numValues), 0);
				p->isCustomizableByUser = true;
				p->isRemovableByUser = true;
				p->saveValueOnly = false;
				valuesCC.addControllable(p);

				numValues = valuesCC.controllables.size();
			}

		}
		
		for (int i = 0; i < numArgs; i++)
		{
			FloatParameter * c = dynamic_cast<FloatParameter *>(valuesCC.controllables[i]);
			if (c != nullptr)
			{
				float value = data[i * 4] + (data[i * 4 + 1] << 8) + (data[i * 4 + 2] << 16) + (data[i * 4 + 3] << 24);
				c->setValue(value);
			}
		}
	}
	break;

	case RAW_COLORS:
	{
		int numArgs = data.size() / 4;

		if (autoAdd->boolValue())
		{
			int numValues = valuesCC.controllables.size();
			while (numValues < numArgs)
			{
				ColorParameter * colP = new ColorParameter("Value " + String(numValues), "Value " + String(numValues));
				colP->isCustomizableByUser = true;
				colP->isRemovableByUser = true;
				colP->saveValueOnly = false;

				valuesCC.addControllable(colP);
				numValues = valuesCC.controllables.size();
			}
		}
		

		for (int i = 0; i < numArgs; i++)
		{
			ColorParameter * c = dynamic_cast<ColorParameter *>(valuesCC.controllables[i]);
			if (c != nullptr)
			{
				Colour col = Colour(data[i * 4], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]);
				c->setColor(col);
			}
		}
	}
	break;
            
    default:
        break;
    }
	
}

void StreamingModule::sendMessage(const String & message)
{
	if (!enabled->boolValue() || !isReadyToSend()) return;
	sendMessageInternal(message);
	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue()) NLOG(niceName, "Sending : " << message);
}

void StreamingModule::sendBytes(Array<uint8> bytes)
{
	if (!enabled->boolValue() || !isReadyToSend()) return;

	if (streamingType->getValueDataAsEnum<StreamingType>() == COBS)
	{
		Array<uint8> sourceBytes = Array<uint8>(bytes);
		cobs_encode(sourceBytes.getRawDataPointer(), bytes.size(), bytes.getRawDataPointer());
	}

	sendBytesInternal(bytes);
	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue()) NLOG(niceName, "Sending " + String(bytes.size()) + " bytes");
}

void StreamingModule::showMenuAndCreateValue(ControllableContainer * container)
{
	StringArray filters = ControllableFactory::getTypesWithout(StringArray(EnumParameter::getTypeStringStatic(), TargetParameter::getTypeStringStatic(), FileParameter::getTypeStringStatic()));
	Controllable * c = ControllableFactory::showFilteredCreateMenu(filters);
	if (c == nullptr) return;

	AlertWindow window("Add a value", "Configure the parameters for this value", AlertWindow::AlertIconType::NoIcon);
	window.addTextEditor("address", "MyValue", "OSC Address");
	window.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	window.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = window.runModalLoop();

	if (result)
	{
		String addString = window.getTextEditorContents("address").replace(" ", "");
		c->setNiceName(addString);
		container->addControllable(c);
	} else
	{
		delete c;
	}
}

void StreamingModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == autoAdd || c == streamingType)
	{
		bool streamingLines = streamingType->getValueDataAsEnum<StreamingType>() == LINES;
		messageStructure->setEnabled(autoAdd->boolValue());
		firstValueIsTheName->setEnabled(streamingLines && autoAdd->boolValue());
	}

	if (c == streamingType)
	{
		while (valuesCC.controllables.size() > 0) valuesCC.removeControllable(valuesCC.controllables[0]);
		buildMessageStructureOptions();
	} else if (c == messageStructure)
	{
		while (valuesCC.controllables.size() > 0) valuesCC.removeControllable(valuesCC.controllables[0]);
	}
}

var StreamingModule::sendStringFromScript(const var::NativeFunctionArgs & a)
{
	StreamingModule * m = getObjectFromJS<StreamingModule>(a);
	if (a.numArguments == 0) return var();
	m->sendMessage(a.arguments[0].toString());
	return var();
}

var StreamingModule::sendBytesFromScript(const var::NativeFunctionArgs & a)
{
	StreamingModule * m = getObjectFromJS<StreamingModule>(a);
	if (a.numArguments == 0) return var();
	Array<uint8> data;
	for (int i = 0; i < a.numArguments; i++)
	{
		if (a.arguments[i].isArray())
		{
			Array<var> * aa = a.arguments[i].getArray();
			for (auto &vaa : *aa) data.add((uint8)(int)vaa);
		} else if (a.arguments[i].isInt() || a.arguments[i].isDouble())
		{
			data.add((uint8)(int)a.arguments[i]);
		}
	}

	m->sendBytes(data);
	return var();
}
