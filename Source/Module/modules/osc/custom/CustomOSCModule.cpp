/*
  ==============================================================================

	CustomOSCModule.cpp
	Created: 29 Oct 2016 7:07:54pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Command/CommandFactory.h"

CustomOSCModule::CustomOSCModule() :
	OSCModule("OSC"),
	autoAdd(nullptr),
	useHierarchy(nullptr),
	autoFeedback(nullptr),
	hierarchyStructureSwitch(false)
{
	includeValuesInSave = true;

	autoAdd = moduleParams.addBoolParameter("Auto Add", "Add automatically any message that is received\nand try to create the corresponding value depending on the message content.", true);
	splitArgs = moduleParams.addBoolParameter("Split Arguments", "If checked, a message with multiple arguments will be exploded in multliple values", false);
	useHierarchy = moduleParams.addBoolParameter("Use Hierarchy", "If checked, incoming messages will be sorted in nested containers instead of 1-level", false);
	autoFeedback = moduleParams.addBoolParameter("Auto Feedback", "If checked, all changed values will be automatically sent back to the outputs", false);
	colorMode = moduleParams.addEnumParameter("Color Send Mode", "The way to send color arguments.\nColor will send a 'r' argument with the encoded color.\nRGB Float will send 3 floats and RGBA Float will send 4 floats.");
	colorMode->addOption("Color", ColorRGBA)->addOption("RGB Float", Float3)->addOption("RGBA Float", Float4);

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &CustomOSCModule::showMenuAndCreateValue;
}

void CustomOSCModule::processMessageInternal(const OSCMessage& msg)
{
	if (autoAdd == nullptr || useHierarchy == nullptr || autoFeedback == nullptr) return;

	String cNiceName = msg.getAddressPattern().toString();
	String cShortName = cNiceName.replaceCharacter('/', ' ');

	Controllable* c = nullptr;
	ControllableContainer* cParentContainer = &valuesCC;

	bool hasWildcards = msg.getAddressPattern().containsWildcards();

	if (useHierarchy->boolValue() && !hasWildcards)
	{
		StringArray addSplit;
		addSplit.addTokens(msg.getAddressPattern().toString(), "/", "");
		addSplit.remove(0);
		cNiceName = addSplit[addSplit.size() - 1];
		cShortName = cNiceName;

		addSplit.remove(addSplit.size() - 1);


		if (addSplit.size() > 0)
		{
			cParentContainer = valuesCC.getControllableContainerForAddress(addSplit);

			if (cParentContainer == nullptr)
			{
				if (autoAdd->boolValue())
				{
					ControllableContainer* childContainer = &valuesCC;

					for (auto& s : addSplit)
					{
						ControllableContainer* cc = childContainer->getControllableContainerByName(s);
						if (cc == nullptr)
						{
							cc = new ControllableContainer(s);
							cc->setCustomShortName(s);
							cc->saveAndLoadRecursiveData = true;
							cc->isRemovableByUser = true;
							cc->includeTriggersInSaveLoad = true;
							childContainer->addChildControllableContainer(cc, true);
						}

						childContainer = cc;
					}

					cParentContainer = childContainer;
				}
				else
				{
					return; //container not found, do nothing
				}
			}
		}
	}


	//jassert(cParentContainer != nullptr);
	//first we remove slashes to allow for simple controllableContainer search


	if (msg.size() > 1 && splitArgs->boolValue()) // Split args on multi type
	{
		for (int i = 0; i < msg.size(); ++i)
		{
			c = cParentContainer->getControllableByName(cShortName + "_" + String(i));

			//OSCAddressPattern address(msg.getAddressPattern().toString() + "_" + String(i));
			//Array<Controllable*> matchCont = getMatchingControllables(address);
			//if (matchCont.size() == 1) c = matchCont[0];

			//if (matchCont.size() > 0)
			//{
			//	for (auto& c : matchCont)
			//	{

			if (c != nullptr) //Args already exists
			{
				Parameter* p = (Parameter*)c;
				switch (c->type)
				{
				case Controllable::BOOL: p->setValue(getFloatArg(msg[i]) >= 1); break;
				case Controllable::FLOAT: p->setValue(getFloatArg(msg[i])); break;
				case Controllable::INT: p->setValue(getIntArg(msg[i])); break;
				case Controllable::STRING: p->setValue(getStringArg(msg[i])); break;
				default:
					break;
				}
			}
			else if (autoAdd->boolValue()) //Args don't exist yet
			{
				String argIAddress = cNiceName + " " + String(i);
				if (msg[i].isInt32())
				{
					c = cParentContainer->addIntParameter(argIAddress, "", msg[i].getInt32());
				}
				else if (msg[i].isFloat32())
				{
					c = cParentContainer->addFloatParameter(argIAddress, "", msg[i].getFloat32());
				}
				else if (msg[i].isString()) c = cParentContainer->addStringParameter(argIAddress, "", msg[i].getString());


				if (c != nullptr) //Args have been sucessfully created 
				{
					c->setCustomShortName(cShortName + "_" + String(i)); //force safeName for search
					c->isCustomizableByUser = true;
					c->isRemovableByUser = true;
					c->saveValueOnly = false;
				}
			}
		}
	}
	else //Standard handling of incoming messages
	{
		Array<Controllable*> matchCont = getMatchingControllables(msg.getAddressPattern());
		if (matchCont.size() == 0 && msg.getAddressPattern().toString().containsChar(' '))
		{
			Controllable* c = valuesCC.getControllableByName(msg.getAddressPattern().toString(), true);
			if (c != nullptr) matchCont.add(c);
		}
		if (matchCont.size() > 0) c = matchCont[0];

		for (auto& c : matchCont)
		{
			if (c == nullptr) continue;

			switch (c->type)
			{
			case Controllable::TRIGGER:
				((Trigger*)c)->trigger();
				break;

			case Controllable::BOOL:
				((Parameter*)c)->setValue(getFloatArg(msg[0]) >= 1); break;
				break;

			case Controllable::FLOAT:
				if (msg.size() >= 1)
				{
					FloatParameter* f = (FloatParameter*)c;
					f->setValue(getFloatArg(msg[0]));
				}
				break;

			case Controllable::INT:
				if (msg.size() >= 1)
				{
					IntParameter* i = (IntParameter*)c;
					i->setValue(getIntArg(msg[0]));
				}
				break;

			case Controllable::STRING:
				if (msg.size() >= 1) ((StringParameter*)c)->setValue(getStringArg(msg[0]));
				break;

			case Controllable::POINT2D:
				if (msg.size() >= 2) ((Point2DParameter*)c)->setPoint(getFloatArg(msg[0]), getFloatArg(msg[1]));
				break;

			case Controllable::POINT3D:
				if (msg.size() >= 3) ((Point3DParameter*)c)->setVector(Vector3D<float>(getFloatArg(msg[0]), getFloatArg(msg[1]), getFloatArg(msg[2])));
				break;

			case Controllable::COLOR:
				if (msg.size() >= 3) ((ColorParameter*)c)->setColor(Colour((uint8)(getFloatArg(msg[0]) * 255), (uint8)(getFloatArg(msg[1]) * 255), (uint8)(getFloatArg(msg[2]) * 255), msg.size() >= 4 ? getFloatArg(msg[3]) : 1));
				break;

			default:
				//not handled
				break;
			}
		}
	}


	//ADDING VALUE

	if (c == nullptr && autoAdd->boolValue() && !hasWildcards) //if auto add, add a new value
	{
		const int numArgs = msg.size();

		switch (numArgs)
		{
		case 0:
			c = new Trigger(cNiceName, "");
			break;

		case 1:
			if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", msg[0].getInt32());
			}
			else if (msg[0].isFloat32())
			{
				c = new FloatParameter(cNiceName, "", msg[0].getFloat32());
			}
			else if (msg[0].isString()) c = new StringParameter(cNiceName, "", msg[0].getString());

			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", getIntArg(msg[0]));
			}
			else if (msg[0].isFloat32())
			{
				c = new Point2DParameter(cNiceName, "");
				((Point2DParameter*)c)->setPoint(getFloatArg(msg[0]), getFloatArg(msg[1]));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", getStringArg(msg[0]));
			}
			break;

		case 3:
			if (msg[0].isInt32()) c = new IntParameter(cNiceName, "", getIntArg(msg[0]), getIntArg(msg[1]), getIntArg(msg[2]));
			else if (msg[0].isFloat32())
			{

				c = new Point3DParameter(cNiceName, "");
				((Point3DParameter*)c)->setVector(getFloatArg(msg[0]), getFloatArg(msg[1]), getFloatArg(msg[2]));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", getStringArg(msg[0]));
			}
			break;

		case 4:
			if (msg[0].isFloat32() && msg[1].isFloat32() && msg[2].isFloat32() && msg[3].isFloat32())
			{
				c = new ColorParameter(cNiceName, "", Colour((uint8)(getFloatArg(msg[0]) * 255), (uint8)(getFloatArg(msg[1]) * 255), (uint8)(getFloatArg(msg[2]) * 255), getFloatArg(msg[3])));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", getStringArg(msg[0]));
			}
			break;

		default:
			//DBG("Message " + cNiceName + "num arguments not handled : " + numArgs);
			return;
		}


		if (c != nullptr)
		{
			c->setCustomShortName(cShortName); //force safeName for search
			c->isCustomizableByUser = true;
			c->isRemovableByUser = true;
			c->saveValueOnly = false;
			//c->setControllableFeedbackOnly(true);

			cParentContainer->addControllable(c);
			cParentContainer->sortControllables();
		}
	}
}

Array<Controllable*> CustomOSCModule::getMatchingControllables(const OSCAddressPattern& address)
{
	Array<Controllable*> matchCont;
	HashMap<String, WeakReference<Controllable>, DefaultHashFunctions, CriticalSection>::Iterator it(controllableAddressMap);
	while (it.next())
	{
		if (it.getValue().wasObjectDeleted()) continue;
		try
		{
			OSCAddress a(it.getKey());
			if (address.matches(a)) matchCont.add(it.getValue());
		}
		catch (...)
		{
			DBG("Error trying to match, name " << it.getKey() << " may not be a valid OSC Address");
		}
	}

	return matchCont;
}

void CustomOSCModule::updateControllableAddressMap()
{
	controllableAddressMap.clear();
	Array<WeakReference<Controllable>> cont = valuesCC.getAllControllables(true);
	for (auto& c : cont)
	{
		if (c.wasObjectDeleted()) continue;
		String address = useHierarchy->boolValue() ? c->getControlAddress(&valuesCC) : c->niceName;
		if (!address.startsWith("/") || address.containsChar(' ')) continue; //don't add values that are not addresses
		controllableAddressMap.set(address, c);
	}
}

void CustomOSCModule::childStructureChanged(ControllableContainer* cc)
{
	ControllableContainer::childStructureChanged(cc);
	if (!isCurrentlyLoadingData && !hierarchyStructureSwitch) updateControllableAddressMap();
}


void CustomOSCModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	OSCModule::onControllableFeedbackUpdateInternal(cc, c);

	if (autoFeedback->boolValue())
	{
		if (isControllableInValuesContainer(c))
		{
			try
			{
				String cAddress = useHierarchy->boolValue() ? c->getControlAddress(&valuesCC) : c->niceName;
				if (!cAddress.startsWith("/"))
				{
					//Not an osc address, not sending
					return;
				}

				OSCMessage m(cAddress);
				if (c->type == Controllable::TRIGGER) sendOSC(m);
				else
				{
					Parameter* p = static_cast<Parameter*>(c);
					if (p != nullptr)
					{
						if (c->type == Controllable::COLOR) m.addArgument(varToColorArgument(p->value));
						else if (p->value.isArray())
						{
							if (Array<var>* arr = p->value.getArray())
							{
								for (auto& aa : *arr) m.addArgument(varToArgument(aa));
							}
						}
						else
						{
							m.addArgument(varToArgument(p->value));
						}
					}
				}

				sendOSC(m);
			}
			catch (OSCFormatError& e)
			{
				NLOGERROR(niceName, "Error sending feedback " << c->niceName << " : " << e.description);
			}

		}
	}
	else if (c == useHierarchy)
	{
		if (!isCurrentlyLoadingData)
		{
			hierarchyStructureSwitch = true;
			Array<WeakReference<Controllable>> cont = valuesCC.getAllControllables(true);
			Array<OSCMessage> msg;
			for (auto& c : cont)
			{
				OSCMessage m = OSCHelpers::getOSCMessageForControllable(c, &valuesCC);
				if (useHierarchy->boolValue()) m.setAddressPattern(c->niceName.replaceCharacter(' ', '_'));
				msg.add(m);
			}

			valuesCC.clear();
			bool tmpAutoAdd = autoAdd->boolValue();
			autoAdd->setValue(true);
			for (auto& m : msg) processMessage(m);
			autoAdd->setValue(tmpAutoAdd);
			hierarchyStructureSwitch = false;

			updateControllableAddressMap();
		}
	}
}

void CustomOSCModule::showMenuAndCreateValue(ControllableContainer* container)
{
	StringArray filters = ControllableFactory::getTypesWithout(StringArray(EnumParameter::getTypeStringStatic(), TargetParameter::getTypeStringStatic(), FileParameter::getTypeStringStatic()));
	Controllable* c = ControllableFactory::showFilteredCreateMenu(filters, true);
	if (c == nullptr) return;

	AlertWindow window("Add a value", "Configure the parameters for this value", AlertWindow::AlertIconType::NoIcon);
	window.addTextEditor("address", "/myValue", "OSC Address");
	window.addButton("OK", 1, KeyPress(KeyPress::returnKey));
	window.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	int result = window.runModalLoop();

	if (result)
	{
		String addString = window.getTextEditorContents("address").replace(" ", "");
		if (!addString.startsWith("/")) addString = "/" + addString;
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
}

void CustomOSCModule::addColorArgumentToMessage(OSCMessage& m, const Colour& c)
{
	ColorMode cm = colorMode->getValueDataAsEnum<ColorMode>();

	if (cm == ColorRGBA) m.addColour(OSCHelpers::getOSCColour(c));
	else
	{
		m.addFloat32(c.getFloatRed());
		m.addFloat32(c.getFloatGreen());
		m.addFloat32(c.getFloatBlue());
		if (cm == Float4) m.addFloat32(c.getFloatAlpha());
	}
}

void CustomOSCModule::afterLoadJSONDataInternal()
{
	OSCModule::afterLoadJSONDataInternal();
	updateControllableAddressMap();
}

void CustomOSCModule::setupModuleFromJSONData(var data)
{
	OSCModule::setupModuleFromJSONData(data);
	autoAdd->setValue(false);
	autoAdd->hideInEditor = true;
	splitArgs->hideInEditor = true;
}