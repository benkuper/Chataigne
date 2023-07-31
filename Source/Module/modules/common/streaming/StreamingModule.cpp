/*
  ==============================================================================

	StreamingModule.cpp
	Created: 5 Jan 2018 10:39:38am
	Author:  Ben

  ==============================================================================
*/
#include "Module/ModuleIncludes.h"

StreamingModule::StreamingModule(const String& name) :
	Module(name)
{
	includeValuesInSave = true;
	setupIOConfiguration(true, true);
	canHandleRouteValues = hasOutput;

	streamingType = moduleParams.addEnumParameter("Protocol", "Protocol for treating the incoming data.\nLines will detect new lines and process messages when it finds one.\nDirect will just consider any incoming data as a new data.\nData255 uses the byte 255 as message split.\nJSON will try to parse the incoming data as a JSON Object.\nCOBS is another messaging protocol.");
	streamingType->addOption("Lines", LINES)->addOption("Direct", DIRECT)->addOption("Raw", RAW)->addOption("Data255", DATA255)->addOption("JSON", TYPE_JSON)->addOption("COBS", COBS);

	autoAdd = moduleParams.addBoolParameter("Auto Add", "If checked, incoming data will be parsed depending on the Message Structure parameter, and if eligible will be added as values", true);
	messageStructure = moduleParams.addEnumParameter("Message Structure", "The expected structure of a message, determining how it should be interpreted to auto create values from it");
	firstValueIsTheName = moduleParams.addBoolParameter("First value is the name", "If checked, the first value of a parsed message will be used to name the value, otherwise each values will be named by their index", true);
	buildMessageStructureOptions();

	defManager->add(CommandDefinition::createDef(this, "", "Send string", &SendStreamStringCommand::create, CommandContext::BOTH));
	defManager->add(CommandDefinition::createDef(this, "", "Send values as string", &SendStreamStringValuesCommand::create, CommandContext::BOTH));
	defManager->add(CommandDefinition::createDef(this, "", "Send raw bytes", &SendStreamRawDataCommand::create, CommandContext::BOTH));
	defManager->add(CommandDefinition::createDef(this, "", "Send custom values", &SendStreamValuesCommand::create, CommandContext::BOTH));
	defManager->add(CommandDefinition::createDef(this, "", "Send hex data", &SendStreamStringCommand::create, CommandContext::BOTH)->addParam("mode", SendStreamStringCommand::DataMode::HEX));

	scriptObject.getDynamicObject()->setMethod(sendId, StreamingModule::sendStringFromScript);
	scriptObject.getDynamicObject()->setMethod(sendToId, StreamingModule::sendStringToFromScript);
	scriptObject.getDynamicObject()->setMethod(sendExcludeId, StreamingModule::sendStringExcludeFromScript);
	scriptObject.getDynamicObject()->setMethod(sendBytesId, StreamingModule::sendBytesFromScript);
	scriptObject.getDynamicObject()->setMethod(sendBytesToId, StreamingModule::sendBytesToFromScript);
	scriptObject.getDynamicObject()->setMethod(sendBytesExcludeId, StreamingModule::sendBytesExcludeFromScript);

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("streaming");


	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &StreamingModule::showMenuAndCreateValue;

	if (hasInput)
	{
		thruManager.reset(new ControllableContainer("Pass-through"));
		thruManager->userCanAddControllables = true;
		thruManager->customUserCreateControllableFunc = &StreamingModule::createThruControllable;
		moduleParams.addChildControllableContainer(thruManager.get());
	}
}

StreamingModule::~StreamingModule()
{

}

void StreamingModule::setAutoAddAvailable(bool value)
{
	if (!value) autoAdd->setValue(false);
	autoAdd->hideInEditor = !value;
	streamingType->hideInEditor = !value;
	messageStructure->hideInEditor = !value;
	firstValueIsTheName->hideInEditor = !value;
}

void StreamingModule::buildMessageStructureOptions()
{
	StreamingType t = streamingType->getValueDataAsEnum<StreamingType>();
	messageStructure->clearOptions();

	switch (t)
	{

	case LINES:
	case DIRECT:
	{
		messageStructure->addOption("Space separated", LINES_SPACE)
			->addOption("Tab separated", LINES_TAB)
			->addOption("Comma (,) separated", LINES_COMMA)
			->addOption("Colon (:) separated", LINES_COLON)
			->addOption("Semicolon (;) separated", LINES_SEMICOLON)
			->addOption("Equals (=) separated", LINES_EQUALS)
			->addOption("No separation (will create only one parameter)", NO_SEPARATION);
	}
	break;

	case RAW:
	case COBS:
	case DATA255:
	{
		messageStructure->addOption("1 value per byte", RAW_1BYTE)->addOption("4x4 (floats)", RAW_FLOATS)->addOption("4x4 (RGBA colors)", RAW_COLORS);
	}
	break;

	default:
		break;
	}
}

void StreamingModule::processDataLine(const String& msg)
{
	if (!enabled->boolValue()) return;

	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = (TargetParameter*)c)
			{
				if (!mt->enabled) continue;
				if (StreamingModule* m = (StreamingModule*)(mt->targetContainer.get()))
				{
					m->sendMessage(msg + "\n"); //add newline as it has been removed when parsing
				}
			}
		}
	}


	const String message = msg.removeCharacters("\r\n");

	inActivityTrigger->trigger();

	if (logIncomingData->boolValue()) NLOG(niceName, "Message received : " << (message.isNotEmpty() ? message : "(Empty message)"));

	if (message.isEmpty()) return;

	processDataLineInternal(message);

	scriptManager->callFunctionOnAllItems(dataEventId, message);

	MessageStructure s = messageStructure->getValueDataAsEnum<MessageStructure>();
	StringArray valuesString;
	String separator;
	switch (s)
	{
	case LINES_SPACE: separator = " "; break;
	case LINES_TAB:   separator = "\t"; break;
	case LINES_COMMA: separator = ","; break;
	case LINES_EQUALS: separator = "="; break;
	case LINES_COLON: separator = ":"; break;
	case LINES_SEMICOLON: separator = ";"; break;
	default:
		break;
	}

	if (s != NO_SEPARATION) valuesString.addTokens(message, separator, "\"");
	else
	{
		if (firstValueIsTheName->boolValue()) valuesString.add("Value");
		valuesString.add(message);
	}

	if (valuesString.size() == 0)
	{
		//LOG("No usable data");
		return;
	}

	if (firstValueIsTheName->boolValue())
	{
		String valueName = valuesString[0];
		int numArgs = valuesString.size() - 1;

		Controllable* c = valuesCC.getControllableByName(valueName, true);

		if (c == nullptr)
		{
			if (!autoAdd->boolValue()) return;

			if (numArgs > 0 && valuesString[1].getFloatValue() == 0 && !valuesString[1].containsChar('0'))
			{
				valuesString.remove(0);
				c = new StringParameter(valueName, valueName, valuesString.joinIntoString(" "));
			}
			else
			{
				switch (numArgs)
				{
				case 0: c = new Trigger(valueName, valueName); break;
				case 1:	c = new FloatParameter(valueName, valueName, valuesString[1].getFloatValue()); break;
				case 2: c = new Point2DParameter(valueName, valueName); ((Point2DParameter*)c)->setPoint(valuesString[1].getFloatValue(), valuesString[2].getFloatValue()); break;
				case 3: c = new Point3DParameter(valueName, valueName); ((Point3DParameter*)c)->setVector(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue()); break;
				case 4: c = new ColorParameter(valueName, valueName, Colour::fromFloatRGBA(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue(), valuesString[4].getFloatValue()));
				default:
				{
					valuesString.remove(0);
					c = new StringParameter(valueName, valueName, valuesString.joinIntoString(" "));
				}

				}
			}

			if (c != nullptr)
			{
				c->isCustomizableByUser = true;
				c->isRemovableByUser = true;
				c->saveValueOnly = false;
				valuesCC.addControllable(c);
			}
		}
		else
		{
			Controllable::Type t = c->type;
			switch (t)
			{
			case Controllable::TRIGGER:
				((Trigger*)c)->trigger();
				break;

			case Controllable::FLOAT:
				if (numArgs >= 1) ((FloatParameter*)c)->setValue(valuesString[1].getFloatValue());
				break;

			case Controllable::INT:
				if (numArgs >= 1) ((IntParameter*)c)->setValue(valuesString[1].getIntValue());
				break;

			case Controllable::POINT2D:
				if (numArgs >= 2) ((Point2DParameter*)c)->setPoint(valuesString[1].getFloatValue(), valuesString[2].getFloatValue());
				break;

			case Controllable::POINT3D:
				if (numArgs >= 3) ((Point3DParameter*)c)->setVector(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue());
				break;

			case Controllable::COLOR:
				if (numArgs >= 4) ((ColorParameter*)c)->setColor(Colour::fromFloatRGBA(valuesString[1].getFloatValue(), valuesString[2].getFloatValue(), valuesString[3].getFloatValue(), valuesString[4].getFloatValue()));
				break;

			case Controllable::STRING:
			{
				valuesString.remove(0);
				if (numArgs >= 1) ((StringParameter*)c)->setValue(valuesString.joinIntoString(" "));
			}
			break;

			default:
				break;

			}
		}

	}
	else
	{
		int numArgs = valuesString.size();

		for (int i = 0; i < numArgs; ++i)
		{
			Controllable* c = valuesCC.getControllableByName("Value " + String(i), true);

			if (c == nullptr)
			{
				if (autoAdd->boolValue())
				{
					if (valuesString[i].getFloatValue() == 0 && !valuesString[i].containsChar('0'))
					{
						c = new StringParameter("Value " + String(i), "Value " + String(i), "");
					}
					else
					{
						c = new FloatParameter("Value " + String(i), "Value " + String(i), 0);
					}

					if (c != nullptr)
					{
						c->isCustomizableByUser = true;
						c->isRemovableByUser = true;
						c->saveValueOnly = false;

						valuesCC.addControllable(c);
					}

				}
			}

			if (c != nullptr)
			{
				switch (c->type)
				{
				case Controllable::FLOAT: ((FloatParameter*)c)->setValue(valuesString[i].getFloatValue()); break;
				case Controllable::INT:((IntParameter*)c)->setValue(valuesString[i].getIntValue()); break;
				case Controllable::STRING: ((StringParameter*)c)->setValue(valuesString[i]); break;
				default:
					((Parameter*)c)->setValue(valuesString[i].getFloatValue()); break;
					break;
				}
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
		for (auto& d : data) msg += "\n" + String(d);
		NLOG(niceName, msg);
	}

	inActivityTrigger->trigger();

	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = (TargetParameter*)c)
			{
				if (!mt->enabled) continue;
				if (StreamingModule* m = (StreamingModule*)(mt->targetContainer.get()))
				{
					m->sendBytes(data);
				}
			}
		}
	}

	processDataBytesInternal(data);

	if (scriptManager->items.size() > 0)
	{
		var args;
		for (auto& d : data) args.append(d);
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
				IntParameter* p = new IntParameter("Value " + String(numValues), "Value " + String(numValues), 0, 0, 255);
				p->isCustomizableByUser = true;
				p->isRemovableByUser = true;
				p->saveValueOnly = false;
				p->hexMode = true;
				valuesCC.addControllable(p);
				numValues = valuesCC.controllables.size();
			}
		}

		for (int i = 0; i < numArgs; ++i)
		{
			IntParameter* c = dynamic_cast<IntParameter*>(valuesCC.controllables[i]);
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
				FloatParameter* p = new FloatParameter("Value " + String(numValues), "Value " + String(numValues), 0);
				p->isCustomizableByUser = true;
				p->isRemovableByUser = true;
				p->saveValueOnly = false;
				valuesCC.addControllable(p);

				numValues = valuesCC.controllables.size();
			}

		}

		for (int i = 0; i < numArgs; ++i)
		{
			FloatParameter* c = dynamic_cast<FloatParameter*>(valuesCC.controllables[i]);
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
				ColorParameter* colP = new ColorParameter("Value " + String(numValues), "Value " + String(numValues));
				colP->isCustomizableByUser = true;
				colP->isRemovableByUser = true;
				colP->saveValueOnly = false;

				valuesCC.addControllable(colP);
				numValues = valuesCC.controllables.size();
			}
		}


		for (int i = 0; i < numArgs; ++i)
		{
			ColorParameter* c = dynamic_cast<ColorParameter*>(valuesCC.controllables[i]);
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

void StreamingModule::processDataJSON(const var& data)
{
	if (!enabled->boolValue()) return;
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "JSON received : " << (!data.isVoid() ? JSON::toString(data) : "(Invalid JSON)"));
	}

	inActivityTrigger->trigger();

	createControllablesFromJSONResult(data, &valuesCC);
}

void StreamingModule::sendMessage(const String& message, var params)
{
	if (!enabled->boolValue()) return;
	if (!isReadyToSend())
	{
		if (logOutgoingData->boolValue()) NLOGWARNING(niceName, "Can't send message, output is not connected");
		return;
	}

	sendMessageInternal(message, params);
	outActivityTrigger->trigger();

	if (logOutgoingData->boolValue()) NLOG(niceName, "Sending : " << message);
}

void StreamingModule::sendBytes(Array<uint8> bytes, var params)
{
	if (!enabled->boolValue()) return;
	if (!isReadyToSend())
	{
		if (logOutgoingData->boolValue()) NLOGWARNING(niceName, "Can't send  data, output is not connected.");
		return;
	}

	if (streamingType->getValueDataAsEnum<StreamingType>() == COBS)
	{
		if (bytes.size() > 255)
		{
			NLOGWARNING(niceName, "Packet length cannot be more than 255 bytes with COBS encoding");
			return;
		}

		int numBytes = bytes.size();
		uint8_t data[255];
		cobs_encode(bytes.getRawDataPointer(), numBytes, data);
		bytes.clear();

		for (int i = 0; i < numBytes + 1; ++i) bytes.add(data[i]);
		bytes.add(0);
	}

	sendBytesInternal(bytes, params);
	outActivityTrigger->trigger();

	if (logOutgoingData->boolValue())
	{
		String s = "Sending " + String(bytes.size()) + " bytes :";
		for (auto& b : bytes) s += "\n0x" + String::toHexString(b);
		NLOG(niceName, s);
	}
}

void StreamingModule::showMenuAndCreateValue(ControllableContainer* container)
{
	StringArray filters = ControllableFactory::getTypesWithout(StringArray(EnumParameter::getTypeStringStatic(), TargetParameter::getTypeStringStatic(), FileParameter::getTypeStringStatic()));

	ControllableFactory::showFilteredCreateMenu(filters,
		[container](Controllable* c)
		{
			if (c == nullptr) return;

			AlertWindow* window = new AlertWindow("Add a value", "Configure the parameters for this value", AlertWindow::AlertIconType::NoIcon);
			window->addTextEditor("address", "MyValue" + String(container->controllables.size() + 1), "OSC Address");
			window->addButton("OK", 1, KeyPress(KeyPress::returnKey));
			window->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

			window->enterModalState(true, ModalCallbackFunction::create([window, container, c](int result)
				{


					if (result)
					{
						String addString = window->getTextEditorContents("address").replace(" ", "");
						c->setNiceName(addString);
						c->isCustomizableByUser = true;
						c->isRemovableByUser = true;
						c->saveValueOnly = false;
						container->addControllable(c);
					}
					else
					{
						delete c;
					}
				}),
				true
					);
		}
	, true);


}

void StreamingModule::createThruControllable(ControllableContainer* cc)
{
	TargetParameter* p = new TargetParameter(cc->getUniqueNameInContainer("Output module 1"), "Target module to send the raw data to", "");
	p->targetType = TargetParameter::CONTAINER;
	p->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<StreamingModule>;
	p->isRemovableByUser = true;
	p->canBeDisabledByUser = true;
	p->saveValueOnly = false;
	cc->addParameter(p);
}

void StreamingModule::createControllablesFromJSONResult(var data, ControllableContainer* container)
{
	if (!data.isObject()) return;

	DynamicObject* dataObject = data.getDynamicObject();
	if (dataObject == nullptr) return;

	NamedValueSet props = dataObject->getProperties();

	for (auto& p : props)
	{
		if (p.value.isArray())
		{
			for (int i = 0; i < p.value.size(); ++i)
			{
				ControllableContainer* cc = container->getControllableContainerByName(String(i), true);
				if (cc == nullptr && autoAdd->boolValue())
				{

					cc = new ControllableContainer(String(i));
					container->addChildControllableContainer(cc, true);
					cc->userCanAddControllables = true;
					cc->saveAndLoadRecursiveData = true;
					cc->saveAndLoadName = true;
				}

				if (cc != nullptr) createControllablesFromJSONResult(p.value[i], cc);
			}

		}
		else if (p.value.isObject())
		{
			ControllableContainer* cc = container->getControllableContainerByName(p.name.toString(), true);
			if (cc == nullptr && autoAdd->boolValue())
			{
				cc = new ControllableContainer(p.name.toString());
				container->addChildControllableContainer(cc, true);
				cc->userCanAddControllables = true;
				cc->saveAndLoadRecursiveData = true;
				cc->saveAndLoadName = true;
			}

			if (cc != nullptr) createControllablesFromJSONResult(p.value, cc);
		}
		else
		{
			Controllable* newC = container->getControllableByName(p.name.toString(), true);
			if (newC == nullptr && autoAdd->boolValue())
			{
				if (p.value.isBool()) newC = new BoolParameter(p.name.toString(), p.name.toString(), false);
				else if (p.value.isDouble()) newC = new FloatParameter(p.name.toString(), p.name.toString(), 0);
				else if (p.value.isInt()) newC = new IntParameter(p.name.toString(), p.name.toString(), 0);
				else if (p.value.isString() || p.value.isVoid()) newC = new StringParameter(p.name.toString(), p.name.toString(), "");
				else if (p.value.isArray())
				{
					if (p.value.size() == 1) newC = new FloatParameter(p.name.toString(), p.name.toString(), 0);
					else if (p.value.size() == 2) newC = new Point2DParameter(p.name.toString(), p.name.toString());
					else if (p.value.size() == 3) newC = new Point3DParameter(p.name.toString(), p.name.toString());
					else if (p.value.size() == 3) newC = new ColorParameter(p.name.toString(), p.name.toString());
				}


				if (newC != nullptr)
				{
					newC->isCustomizableByUser = true;
					newC->isRemovableByUser = true;
					newC->isSavable = true;
					newC->saveValueOnly = false;
					container->addControllable(newC);
				}
			}

			if (newC != nullptr)
			{
				if (newC->type == Controllable::TRIGGER && (int)p.value != 0) ((Trigger*)newC)->trigger();
				else
				{
					Parameter* param = dynamic_cast<Parameter*>(newC);
					if (param != nullptr) param->setValue(p.value.isVoid() ? "" : p.value, false, true);
				}
			}
		}
	}
}

void StreamingModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
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
		buildMessageStructureOptions();
	}
}

void StreamingModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	for (auto& v : valuesCC.controllables) v->isCustomizableByUser = true;

	if (thruManager != nullptr)
	{
		//thruManager->loadJSONData(data.getProperty("thru", var()));
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = dynamic_cast<TargetParameter*>(c))
			{
				mt->targetType = TargetParameter::CONTAINER;
				mt->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<StreamingModule>;
				mt->isRemovableByUser = true;
				mt->canBeDisabledByUser = true;
			}
		}
	}
}

var StreamingModule::sendStringFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 1)) return false;
	String s = getStringFromArgs(a, 0);
	m->sendMessage(s);
	return var();
}

var StreamingModule::sendBytesFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 1)) return false;
	Array<uint8> data = getByteFromArgs(a, 0);
	m->sendBytes(data);
	return var();
}


var StreamingModule::sendStringToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 3)) return false;
	String s = getStringFromArgs(a, 1);
	m->sendMessage(s, getToExcludeParamObject(a, "include"));
	return var();
}

var StreamingModule::sendStringExcludeFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 2)) return false;
	String s = getStringFromArgs(a, 1);
	m->sendMessage(s, getToExcludeParamObject(a, "exclude"));
	return var();
}

var StreamingModule::sendBytesToFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 2)) return false;
	Array<uint8> data = getByteFromArgs(a, 1);
	m->sendBytes(data, getToExcludeParamObject(a, "include"));
	return var();
}

var StreamingModule::sendBytesExcludeFromScript(const var::NativeFunctionArgs& a)
{
	StreamingModule* m = getObjectFromJS<StreamingModule>(a);
	if (!checkNumArgs(m->niceName, a, 2)) return false;
	Array<uint8> data = getByteFromArgs(a, 1);
	m->sendBytes(data, getToExcludeParamObject(a, "exclude"));
	return var();
}

var StreamingModule::getToExcludeParamObject(const var::NativeFunctionArgs& a, const String& propName)
{
	var result(new DynamicObject());
	var list;

	if (a.arguments[0].isString()) list.append(a.arguments[0].toString());
	else if (a.arguments[0].isArray()) list = a.arguments[0];

	result.getDynamicObject()->setProperty(propName, list);

	return result;
}

String StreamingModule::getStringFromArgs(const var::NativeFunctionArgs& a, int offset)
{
	String result = "";
	if (a.arguments[offset].isObject()) result = JSON::toString(a.arguments[offset]);
	else
	{
		for (int i = offset; i < a.numArguments; ++i)
		{
			if (a.arguments[i].isArray() && a.arguments[i].size() > 0)
			{
				for (int j = 0; j < a.arguments[i].size(); j++)
				{
					result += (result.isNotEmpty() ? " " : "") + a.arguments[i][j].toString();
				}
			}
			else
			{
				result += (result.isNotEmpty() ? " " : "") + a.arguments[i].toString();
			}
		}
	}

	return result;
}

Array<uint8> StreamingModule::getByteFromArgs(const var::NativeFunctionArgs& a, int offset)
{
	Array<uint8> data;
	for (int i = offset; i < a.numArguments; ++i)
	{
		if (a.arguments[i].isArray())
		{
			Array<var>* aa = a.arguments[i].getArray();
			for (auto& vaa : *aa) data.add((uint8)(int)vaa);
		}
		else if (a.arguments[i].isInt() || a.arguments[i].isDouble() || a.arguments[i].isInt64() || a.arguments[i].isBool())
		{
			data.add((uint8)(int)a.arguments[i]);
		}
	}

	return data;
}

StreamingModule::StreamingRouteParams::StreamingRouteParams(Module* sourceModule, Controllable* c)
{
	prefix = addStringParameter("Prefix", "Prefix to put before the actual value", c->shortName + " ");
	appendNL = addBoolParameter("NL", "Append NL (New Line) at the end", false);
	appendCR = addBoolParameter("CR", "Append CR (Charriot Return) at the end", false);
}


void StreamingModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	StreamingRouteParams* op = dynamic_cast<StreamingRouteParams*>(p);

	String s = op->prefix->stringValue();
	if (c->type != Controllable::TRIGGER)
	{
		s += dynamic_cast<Parameter*>(c)->stringValue();
	}

	s += op->appendCR->boolValue() ? "\r" : "";
	s += op->appendNL->boolValue() ? "\n" : "";

	sendMessage(s);
}
