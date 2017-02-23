/*
  ==============================================================================

    CustomOSCCommandArgument.cpp
    Created: 22 Feb 2017 8:51:30am
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandArgument.h"
#include "CustomOSCCommandArgumentEditor.h"

CustomOSCCommandArgument::CustomOSCCommandArgument(const String &name, Parameter * _p, bool _mappingEnabled) :
	BaseItem(name, false),
	param(_p),
	mappingEnabled(_mappingEnabled),
	useForMapping(nullptr)
{
	isSelectable = false;
	param->isCustomizableByUser = true;
	param->saveValueOnly = false;

	jassert(param != nullptr);
	addControllable(param);

	//argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	if(mappingEnabled) useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
}



var CustomOSCCommandArgument::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", param->getTypeString());
	data.getDynamicObject()->setProperty("param", param->getJSONData());
	return data;
}

void CustomOSCCommandArgument::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == useForMapping)
	{
		argumentListeners.call(&ArgumentListener::useForMappingChanged, this);
	}
}



InspectableEditor * CustomOSCCommandArgument::getEditor(bool isRoot)
{
	return new CustomOSCCommandArgumentEditor(this, isRoot);
}