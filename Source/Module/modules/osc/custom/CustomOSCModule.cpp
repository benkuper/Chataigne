/*
  ==============================================================================

	CustomOSCModule.cpp
	Created: 29 Oct 2016 7:07:54pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

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
	colorMode->addOption("Color", OSCHelpers::ColorRGBA)->addOption("RGB Float", OSCHelpers::Float3)->addOption("RGBA Float", OSCHelpers::Float4);

	boolMode = moduleParams.addEnumParameter("Boolean Send Mode", "The way to send boolean arguments.\nInt will send a 'i' argument with the value to 0 or 1\nFloat will send a 'f' argument with the value to 0.0 or 1.0\nT-F will send T or F arguments for True or False");
	boolMode->addOption("Int", OSCHelpers::Int)->addOption("Float", OSCHelpers::Float)->addOption("T-F", OSCHelpers::TF);

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &CustomOSCModule::showMenuAndCreateValue;

}

OSCHelpers::ColorMode CustomOSCModule::getColorMode()
{
	return colorMode->getValueDataAsEnum<OSCHelpers::ColorMode>();
}

OSCHelpers::BoolMode CustomOSCModule::getBoolMode()
{
	return boolMode->getValueDataAsEnum<OSCHelpers::BoolMode>();
}

void CustomOSCModule::processMessageInternal(const OSCMessage& msg)
{
	if (autoAdd == nullptr || useHierarchy == nullptr || autoFeedback == nullptr) return;

	String cNiceName = msg.getAddressPattern().toString();
	String cShortName = cNiceName.replaceCharacter('/', '_');

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
			cParentContainer = valuesCC.getControllableContainerForAddress(addSplit, true, false, true, true);

			if (cParentContainer == nullptr)
			{
				if (autoAdd->boolValue())
				{
					ControllableContainer* childContainer = &valuesCC;

					for (auto& s : addSplit)
					{
						ControllableContainer* cc = childContainer->getControllableContainerByName(s, true, true);
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
			c = cParentContainer->getControllableByName(cShortName + "_" + String(i), true, true);

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
				case Controllable::BOOL: p->setValue(OSCHelpers::getBoolArg(msg[i])); break;
				case Controllable::FLOAT: p->setValue(OSCHelpers::getFloatArg(msg[i])); break;
				case Controllable::INT: p->setValue(OSCHelpers::getIntArg(msg[i])); break;
				case Controllable::STRING: p->setValue(OSCHelpers::getStringArg(msg[i])); break;
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
		Array<WeakReference<Controllable> > matchCont = getMatchingControllables(msg.getAddressPattern());
		if (matchCont.size() == 0 || cNiceName.containsChar(' '))
		{
			Controllable* c = cParentContainer->getControllableByName(cNiceName, true, true);
			if (c != nullptr) matchCont.add(c);
		}
		if (matchCont.size() > 0) c = matchCont[0];

		for (auto& wc : matchCont)
		{
			if (wc == nullptr || wc.wasObjectDeleted()) continue;
			Controllable* c = wc.get();
			if (c == nullptr) continue;

			switch (c->type)
			{
			case Controllable::TRIGGER:
				((Trigger*)c)->trigger();
				break;

			case Controllable::BOOL:
				((Parameter*)c)->setValue(OSCHelpers::getBoolArg(msg[0])); break;
				break;

			case Controllable::FLOAT:
				if (msg.size() >= 1)
				{
					FloatParameter* f = (FloatParameter*)c;
					f->setValue(OSCHelpers::getFloatArg(msg[0]));
				}
				break;

			case Controllable::INT:
				if (msg.size() >= 1)
				{
					IntParameter* i = (IntParameter*)c;
					i->setValue(OSCHelpers::getIntArg(msg[0]));
				}
				break;

			case Controllable::STRING:
				if (msg.size() >= 1) ((StringParameter*)c)->setValue(OSCHelpers::getStringArg(msg[0]));
				break;

			case Controllable::POINT2D:
				if (msg.size() >= 2) ((Point2DParameter*)c)->setPoint(OSCHelpers::getFloatArg(msg[0]), OSCHelpers::getFloatArg(msg[1]));
				break;

			case Controllable::POINT3D:
				if (msg.size() >= 3) ((Point3DParameter*)c)->setVector(Vector3D<float>(OSCHelpers::getFloatArg(msg[0]), OSCHelpers::getFloatArg(msg[1]), OSCHelpers::getFloatArg(msg[2])));
				break;

			case Controllable::COLOR:
				if (msg.size() >= 3) ((ColorParameter*)c)->setColor(Colour((uint8)(OSCHelpers::getFloatArg(msg[0]) * 255), (uint8)(OSCHelpers::getFloatArg(msg[1]) * 255), (uint8)(OSCHelpers::getFloatArg(msg[2]) * 255), msg.size() >= 4 ? OSCHelpers::getFloatArg(msg[3]) : 1));
				else if (msg.size() > 0 && msg[0].isColour()) ((ColorParameter*)c)->setColor(OSCHelpers::getColourFromOSC(msg[0].getColour()));
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
			if (msg[0].isTorF())
			{
				c = new BoolParameter(cNiceName, "", msg[0].getBool());
			}
			else if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", msg[0].getInt32());
			}
			else if (msg[0].isFloat32())
			{
				c = new FloatParameter(cNiceName, "", msg[0].getFloat32());
			}
			else if (msg[0].isString()) c = new StringParameter(cNiceName, "", msg[0].getString());
			else if (msg[0].isColour()) c = new ColorParameter(cNiceName, "", OSCHelpers::getColourFromOSC(msg[0].getColour()));
			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", OSCHelpers::getIntArg(msg[0]));
			}
			else if (msg[0].isFloat32())
			{
				c = new Point2DParameter(cNiceName, "");
				((Point2DParameter*)c)->setPoint(OSCHelpers::getFloatArg(msg[0]), OSCHelpers::getFloatArg(msg[1]));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", OSCHelpers::getStringArg(msg[0]));
			}
			break;

		case 3:
			if (msg[0].isInt32()) c = new IntParameter(cNiceName, "", OSCHelpers::getIntArg(msg[0]), OSCHelpers::getIntArg(msg[1]), OSCHelpers::getIntArg(msg[2]));
			else if (msg[0].isFloat32())
			{

				c = new Point3DParameter(cNiceName, "");
				((Point3DParameter*)c)->setVector(OSCHelpers::getFloatArg(msg[0]), OSCHelpers::getFloatArg(msg[1]), OSCHelpers::getFloatArg(msg[2]));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", OSCHelpers::getStringArg(msg[0]));
			}
			break;

		case 4:
			if (msg[0].isFloat32() && msg[1].isFloat32() && msg[2].isFloat32() && msg[3].isFloat32())
			{
				c = new ColorParameter(cNiceName, "", Colour((uint8)(OSCHelpers::getFloatArg(msg[0]) * 255), (uint8)(OSCHelpers::getFloatArg(msg[1]) * 255), (uint8)(OSCHelpers::getFloatArg(msg[2]) * 255), OSCHelpers::getFloatArg(msg[3])));
			}
			else if (msg[0].isString())
			{
				c = new StringParameter(cNiceName, "", OSCHelpers::getStringArg(msg[0]));
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

Array<WeakReference<Controllable>> CustomOSCModule::getMatchingControllables(const OSCAddressPattern& address)
{
	GenericScopedLock lock(controllableAddressMap.getLock());

	Array<WeakReference<Controllable>> matchCont;
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
	GenericScopedLock lock(controllableAddressMap.getLock());
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
	if (!isCurrentlyLoadingData && !hierarchyStructureSwitch)
	{
		updateControllableAddressMap();
	}
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
				else OSCHelpers::addArgumentsForParameter(m, (Parameter*)c, getBoolMode(), getColorMode());

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

	ControllableFactory::showFilteredCreateMenu(filters, [container](Controllable* c)
		{
			if (c == nullptr) return;

			AlertWindow* window = new AlertWindow("Add a value", "Configure the parameters for this value", AlertWindow::AlertIconType::NoIcon);
			window->addTextEditor("address", "/myValue" + String(container->controllables.size(
			) + 1), "OSC Address");
			window->addButton("OK", 1, KeyPress(KeyPress::returnKey));
			window->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

			window->enterModalState(true, ModalCallbackFunction::create([window, container, c](int result)
				{

					if (result)
					{
						String addString = window->getTextEditorContents("address").replace(" ", "");
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
				}),
				true
					);
		}
	, true);

}


void CustomOSCModule::afterLoadJSONDataInternal()
{
	OSCModule::afterLoadJSONDataInternal();
	updateControllableAddressMap();
}

void CustomOSCModule::setupModuleFromJSONData(var data)
{
	if (outputManager != nullptr)
	{
		var outputsData = data["defaults"]["OSC Outputs"];
		if (outputsData.isObject())
		{
			outputManager->clear();
			NamedValueSet nvSet = outputsData.getDynamicObject()->getProperties();
			for (auto& nv : nvSet)
			{
				if (nv.value.isObject())
				{
					OSCOutput* o = outputManager->addItem(nullptr, var(), false);
					o->setNiceName(nv.name.toString());
					loadDefaultsParameterValuesForContainer(nv.value, o);
				}
				else
				{

					if (Parameter* p = static_cast<Parameter*>(outputManager->getControllableByName(nv.name.toString(), true)))
					{
						p->setValue(nv.value);
					}
				}
			}
		}
	}

	OSCModule::setupModuleFromJSONData(data);
	autoAdd->setValue(false);
	autoAdd->hideInEditor = true;
	splitArgs->hideInEditor = true;
	setupIOConfiguration(hasInput && receiveCC != nullptr && receiveCC->enabled->boolValue(), hasOutput && outputManager != nullptr && outputManager->enabled->boolValue());
}