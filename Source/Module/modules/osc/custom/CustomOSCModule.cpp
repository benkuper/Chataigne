/*
  ==============================================================================

    CustomOSCModule.cpp
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCModule.h"
#include "Common/Command/CommandFactory.h"


CustomOSCModule::CustomOSCModule() :
	OSCModule("OSC"),
	autoAdd(nullptr)
{
	includeValuesInSave = true;

	autoAdd = moduleParams.addBoolParameter("Auto Add", "Add automatically any message that is received\nand try to create the corresponding value depending on the message content.", true);
	splitArgs = moduleParams.addBoolParameter("Split Arguments", "If checked, a message with multiple arguments will be exploded in multliple values", false);
	useHierarchy = moduleParams.addBoolParameter("Use Hierarchy", "If checked, incoming messages will be sorted in nested containers instead of 1-level", false);
	autoFeedback = moduleParams.addBoolParameter("Auto Feedback", "If checked, all changed values will be automatically sent back to the outputs", false);

	valuesCC.userCanAddControllables = true;
	valuesCC.customUserCreateControllableFunc = &CustomOSCModule::showMenuAndCreateValue;
}

void CustomOSCModule::processMessageInternal(const OSCMessage & msg)
{
	if (autoAdd == nullptr) return;


	String cNiceName = msg.getAddressPattern().toString();
	String cShortName = cNiceName.replaceCharacters("/", "_");
	Controllable* c = nullptr;
	ControllableContainer* cParentContainer = &valuesCC;

	if (useHierarchy->boolValue())
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
					cParentContainer = &valuesCC;
					for (auto& s : addSplit)
					{
						ControllableContainer* cc = cParentContainer->getControllableContainerByName(s);
						if (cc == nullptr)
						{
							cc = new ControllableContainer(s);
							cc->saveAndLoadRecursiveData = true;
							cc->isRemovableByUser = true;
							cc->includeTriggersInSaveLoad = true;
							cParentContainer->addChildControllableContainer(cc, true);
							cParentContainer = cc;
						}
					}
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


	if(msg.size() > 1 && splitArgs->boolValue()) // Split args on multi type
	{
		for (int i = 0; i < msg.size(); ++i) 
		{
			c = cParentContainer->getControllableByName(cShortName+"_"+String(i));
			Parameter* p = (Parameter*)c;
			if (c != nullptr) //Args already exists
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
			} else if(autoAdd->boolValue())//Args don't exist yet
			{
				String argIAddress = cNiceName + " " + String(i);
				if (msg[i].isInt32())
				{
					c = cParentContainer->addIntParameter(argIAddress, "", msg[i].getInt32());
				} else if (msg[i].isFloat32())
				{
					c = cParentContainer->addFloatParameter(argIAddress, "", msg[i].getFloat32());
				} else if (msg[i].isString()) c = cParentContainer->addStringParameter(argIAddress, "", msg[i].getString());


				if (c != nullptr) //Args have been sucessfully created 
				{
					c->setCustomShortName(cShortName + "_" + String(i)); //force safeName for search
					c->isCustomizableByUser = true;
					c->isRemovableByUser = true;
					c->saveValueOnly = false;
				}
			}
		}
	} else //Standard handling of incoming messages
	{
		c = cParentContainer->getControllableByName(cShortName);

		
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
					f->setValue(getFloatArg(msg[0]));
				}
				break;

			case Controllable::INT:
				if (msg.size() >= 1)
				{
					IntParameter *i = (IntParameter *)c;
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

			case Controllable::COLOR:
				if (msg.size() >= 3) ((ColorParameter *)c)->setColor(Colour((uint8)(getFloatArg(msg[0]) * 255), (uint8)(getFloatArg(msg[1]) * 255), (uint8)(getFloatArg(msg[2]) * 255), msg.size() >= 4?getFloatArg(msg[3]):1));
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
			if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", msg[0].getInt32());
			} else if (msg[0].isFloat32())
			{
				c = new FloatParameter(cNiceName, "", msg[0].getFloat32());
			} else if (msg[0].isString()) c = new StringParameter(cNiceName, "", msg[0].getString());

			break;

		case 2:
			//duplicate because may have other mechanism
			if (msg[0].isInt32())
			{
				c = new IntParameter(cNiceName, "", getIntArg(msg[0]));
			} else if (msg[0].isFloat32())
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

		case 4:
			if (msg[0].isFloat32() && msg[1].isFloat32() && msg[2].isFloat32() && msg[3].isFloat32())
			{
				c = new ColorParameter(cNiceName, "", Colour((uint8)(getFloatArg(msg[0]) * 255), (uint8)(getFloatArg(msg[1]) * 255), (uint8)(getFloatArg(msg[2]) * 255), getFloatArg(msg[3])));
			} else if (msg[0].isString())
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


void CustomOSCModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	OSCModule::onControllableFeedbackUpdateInternal(cc, c); 

	if (autoFeedback->boolValue())
	{
		if(isControllableInValuesContainer(c))
		{
			try
			{
				OSCMessage m(c->niceName);
				if (c->type == Controllable::TRIGGER) sendOSC(m);
				else
				{
					Parameter * p = static_cast<Parameter *>(c);
					if (p != nullptr)
					{
						if (c->type == Controllable::COLOR) m.addArgument(varToColorArgument(p->value));
						else if (p->value.isArray())
						{

							Array<var>* arr = p->value.getArray();
							for (auto& aa : *arr) m.addArgument(varToArgument(aa));
						}
						else
						{
							m.addArgument(varToArgument(p->value));
						}
					}
				}

				sendOSC(m);
			}
			catch (OSCFormatError &e)
			{
				NLOGERROR(niceName, "Error sending feedback " << c->niceName << " : " << e.description);
			}
			
		}
	}
	
}

void CustomOSCModule::showMenuAndCreateValue(ControllableContainer * container)
{
	StringArray filters = ControllableFactory::getTypesWithout(StringArray(EnumParameter::getTypeStringStatic(), TargetParameter::getTypeStringStatic(), FileParameter::getTypeStringStatic()));
	Controllable * c = ControllableFactory::showFilteredCreateMenu(filters, true);
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

/*
InspectableEditor * CustomOSCModule::getEditor(bool isRoot)
{
	return new ModuleEditor(this,isRoot);
}
*/

void CustomOSCModule::setupModuleFromJSONData(var data)
{
	OSCModule::setupModuleFromJSONData(data);
	autoAdd->setValue(false);
	autoAdd->hideInEditor = true;
	splitArgs->hideInEditor = true;
}