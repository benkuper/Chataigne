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

	if (addressHasWildcards && wildcardsContainer != nullptr)
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
			OSCHelpers::addArgumentsForParameter(m, p, oscModule->getBoolMode(), oscModule->getColorMode(), pVal);
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

var CustomOSCCommand::getJSONData(bool includeNonOverriden)
{
	var data = BaseCommand::getJSONData(includeNonOverriden); //do not inherit OSC:Command to avoid saving "arguments"
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
