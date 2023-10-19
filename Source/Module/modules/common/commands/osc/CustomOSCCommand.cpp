/*
  ==============================================================================

	CustomOSCCommand.cpp
	Created: 3 Nov 2016 12:41:23pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

CustomOSCCommand::CustomOSCCommand(IOSCSenderModule* module, CommandContext context, var params, Multiplex* multiplex) :
	OSCCommand(module, context, params, multiplex),
	addressHasWildcards(false)
{
	//autoLoadPreviousCommandData = true;

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

	if (addressHasWildcards)
	{
		for (auto& w : wildcardsMap)
		{
			if (CustomValuesCommandArgument* a = wildcardsContainer->getItemWithName(w, true, true))
			{
				addrString = addrString.replace("{" + w + "}", a->getLinkedValue(multiplexIndex).toString());
			}
		}
	}

	try
	{
		OSCMessage m(addrString);

		for (auto& a : customValuesManager->items)
		{
			Parameter* p = a->param;
			var pVal = a->getLinkedValue(multiplexIndex);

			if (p == nullptr) continue;
			switch (p->type)
			{
			case Controllable::BOOL: OSCHelpers::addBoolArgumentToMessage(m, pVal, oscModule->getBoolMode()); break;
			case Controllable::INT: m.addInt32((int)pVal); break;
			case Controllable::FLOAT: m.addFloat32((float)pVal); break;
			case Controllable::STRING: m.addString(pVal.toString()); break;
			case Controllable::COLOR: OSCHelpers::addColorArgumentToMessage(m, Colour::fromFloatRGBA(pVal[0], pVal[1], pVal[2], pVal[3]), oscModule->getColorMode()); break;

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
	}
	catch (const OSCFormatError&)
	{
		NLOGERROR("OSC", "Address is invalid :\n" << addrString << " addresses should always start with a forward slash");
		return;
	}
}

void CustomOSCCommand::updateWildcardsMap(const String& address)
{
	wildcardsMap.clear();

	std::string s = address.toStdString();
	std::regex source_regex("\\{(\\w+)\\}");

	auto source_begin = std::sregex_iterator(s.begin(), s.end(), source_regex);
	auto source_end = std::sregex_iterator();

	if (std::distance(source_begin, source_end) == 0)
	{
		addressHasWildcards = false;
		return;
	}

	for (std::sregex_iterator i = source_begin; i != source_end; ++i)
	{
		std::smatch m = *i;
		wildcardsMap.add(m[1].str());
	}

	addressHasWildcards = wildcardsMap.size() > 0;
}

void CustomOSCCommand::onContainerParameterChanged(Parameter* p)
{
	OSCCommand::onContainerParameterChanged(p);
	if (p == address)
	{
		updateWildcardsMap(address->stringValue());

		if (addressHasWildcards)
		{
			if (wildcardsContainer == nullptr)
			{
				wildcardsContainer.reset(new CustomValuesCommandArgumentManager("Address Wildcards", context == MAPPING, linkedTemplate != nullptr, multiplex));
				addChildControllableContainer(wildcardsContainer.get());
				wildcardsContainer->addBaseManagerListener(this);
			}
		}
		else
		{
			if (wildcardsContainer != nullptr)
			{
				wildcardsContainer->removeBaseManagerListener(this);
				removeChildControllableContainer(wildcardsContainer.get());;
				wildcardsContainer.reset();
			}
		}

		if (!isCurrentlyLoadingData) commandListeners.call(&CommandListener::commandContentChanged);
	}
}

void CustomOSCCommand::itemAdded(CustomValuesCommandArgument* i)
{
	OSCCommand::itemAdded(i);
	if (isCurrentlyLoadingData) return;
	if (i->parentContainer == wildcardsContainer.get()) commandListeners.call(&CommandListener::commandContentChanged);
}

void CustomOSCCommand::itemsAdded(Array<CustomValuesCommandArgument*> items)
{
	OSCCommand::itemsAdded(items);

	if (isCurrentlyLoadingData) return;
	for (auto& i : items)
	{
		if (i->parentContainer == wildcardsContainer.get())
		{
			commandListeners.call(&CommandListener::commandContentChanged);
			break;
		}
	}
}

void CustomOSCCommand::updateMappingInputValue(var value, int multiplexIndex)
{
	OSCCommand::updateMappingInputValue(value, multiplexIndex);
	if (wildcardsContainer != nullptr)
	{
		for (auto& a : wildcardsContainer->items) a->paramLink->updateMappingInputValue(value, multiplexIndex);
	}
}

void CustomOSCCommand::setInputNamesFromParams(Array<WeakReference<Parameter>> outParams)
{
	OSCCommand::setInputNamesFromParams(outParams);
	if (wildcardsContainer != nullptr) wildcardsContainer->setInputNames(inputNames);
}

var CustomOSCCommand::getJSONData()
{
	var data = BaseCommand::getJSONData(); //do not inherit OSC:Command to avoid saving "arguments"
	var customValuesData = customValuesManager->getJSONData();
	if (!customValuesData.isVoid()) data.getDynamicObject()->setProperty("argManager", customValuesData);
	if (wildcardsContainer != nullptr) data.getDynamicObject()->setProperty(wildcardsContainer->shortName, wildcardsContainer->getJSONData());
	return data;
}

void CustomOSCCommand::loadJSONDataInternal(var data)
{
	OSCCommand::loadJSONDataInternal(data);
	customValuesManager->loadJSONData(data.getProperty("argManager", var()), true);
	if (wildcardsContainer != nullptr) wildcardsContainer->loadJSONData(data.getProperty(wildcardsContainer->shortName, var()), true);
}
