/*
  ==============================================================================

    CustomOSCCommand.cpp
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

CustomOSCCommand::CustomOSCCommand(CustomOSCModule * module, CommandContext context, var params, Multiplex * multiplex) :
	OSCCommand(module, context, params, multiplex),
	customOSCModule(module),
	wildcardsContainer("Address Parameters")
{
	address->setControllableFeedbackOnly(false);
	address->isSavable = true;

	removeChildControllableContainer(&argumentsContainer);
	
	setUseCustomValues(true);
}

CustomOSCCommand::~CustomOSCCommand()
{
	//masterReference.clear();
}


void CustomOSCCommand::triggerInternal(int multiplexIndex)
{
	if (oscModule == nullptr) return;
	
	BaseCommand::triggerInternal(multiplexIndex);

	String addrString = getLinkedValue(address, multiplexIndex);
	
	try
	{
		OSCMessage m(addrString);

		for (auto &a : customValuesManager->items)
		{
			Parameter * p = a->param;
			var pVal = a->getLinkedValue(multiplexIndex);

			if (p == nullptr) continue;
			switch (p->type)
			{
			case Controllable::BOOL: m.addInt32(pVal ? 1 : 0); break;
			case Controllable::INT: m.addInt32((int)pVal); break;
			case Controllable::FLOAT: m.addFloat32((float)pVal); break;
			case Controllable::STRING: m.addString(pVal.toString()); break;
			case Controllable::COLOR: customOSCModule->addColorArgumentToMessage(m, Colour::fromFloatRGBA(pVal[0], pVal[1], pVal[2], pVal[3])); break;

			case Controllable::POINT2D:
				m.addFloat32(pVal[0]);
				m.addFloat32(pVal[1]);
				break;
			case Controllable::POINT3D:
				m.addFloat32(pVal[0]);
				m.addFloat32(pVal[1]);
				m.addFloat32(pVal[2]);
				break;

			default:
				//not handle
				break;

			}
		}
        oscModule->sendOSC(m);
	}catch (const OSCFormatError &)
	{
		NLOG("OSC", "Address is invalid :\n" << addrString);
		return;
	}
}

void CustomOSCCommand::onContainerParameterChanged(Parameter * p)
{
	OSCCommand::onContainerParameterChanged(p);
	if (p == address)
	{


		if (wildcardsContainer.items.size() == 0)
		{
			if (wildcardsContainer.parentContainer == this) removeChildControllableContainer(&wildcardsContainer);
		} else
		{
			if (wildcardsContainer.parentContainer != this) addChildControllableContainer(&wildcardsContainer);
		}
	}
}


var CustomOSCCommand::getJSONData()
{
	var data = BaseCommand::getJSONData(); //do not inherit OSC:Command to avoid saving "arguments"
	var customValuesData = customValuesManager->getJSONData();
	if(!customValuesData.isVoid()) data.getDynamicObject()->setProperty("argManager", customValuesData);
	return data;
}

void CustomOSCCommand::loadJSONDataInternal(var data)
{
	OSCCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("argManager", var()), true);
}
