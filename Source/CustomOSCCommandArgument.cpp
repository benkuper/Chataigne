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
	useForMapping(nullptr),
	mappingEnabled(_mappingEnabled)
{
	editorCanBeCollapsed = false;

	isSelectable = false;
	param->isCustomizableByUser = true;
	param->saveValueOnly = false;

	jassert(param != nullptr);
	addControllable(param);

	//argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	if (mappingEnabled)
	{
		useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
		useForMapping->hideInEditor = true;
		useForMapping->forceSaveValue = true;
	}

	param->hideInEditor = true;
}



var CustomOSCCommandArgument::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("param", param->getJSONData());
	return data;
}

void CustomOSCCommandArgument::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	param->loadJSONData(data.getProperty("param", var()));
}

void CustomOSCCommandArgument::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == useForMapping)
	{
		argumentListeners.call(&ArgumentListener::useForMappingChanged, this);
	}
}

String CustomOSCCommandArgument::getTypeString() const
{
	return param->getTypeString();
}



InspectableEditor * CustomOSCCommandArgument::getEditor(bool isRoot)
{
	return new CustomOSCCommandArgumentEditor(this, isRoot);
}
