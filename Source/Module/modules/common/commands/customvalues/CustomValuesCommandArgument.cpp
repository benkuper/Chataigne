/*
  ==============================================================================

    CustomValuesCommandArgument.cpp
    Created: 22 Feb 2017 8:51:30am
    Author:  Ben

  ==============================================================================
*/

#include "CustomValuesCommandArgument.h"
#include "ui/CustomValuesCommandArgumentEditor.h"

CustomValuesCommandArgument::CustomValuesCommandArgument(const String &name, Parameter * _p, bool _mappingEnabled, bool templateMode) :
	BaseItem(name, false),
	param(_p),
	editable(nullptr),
    useForMapping(nullptr),
    mappingEnabled(_mappingEnabled),
	templateMode(templateMode),
	linkedTemplate(nullptr)
{
	editorCanBeCollapsed = false;

	isSelectable = false;
	
	
	jassert(param != nullptr);
	addControllable(param);

	param->isCustomizableByUser = templateMode;
	
	param->forceSaveValue = true;
	param->saveValueOnly = !templateMode;
	
	if (templateMode)
	{
		editable = addBoolParameter("Editable", "If unchecked, this parameter will not be editable when instantiating this template command", true);
		editable->hideInEditor = true;
	}

	//argumentName = addStringParameter("Argument name", "Name for the argument", "Arg");
	if (mappingEnabled)
	{
		useForMapping = addBoolParameter("Use for mapping", "Check this to automatically set its value when used in a mapping flow.", false);
		useForMapping->hideInEditor = true;
		useForMapping->forceSaveValue = true;
	}

	param->hideInEditor = true;

}

CustomValuesCommandArgument::~CustomValuesCommandArgument()
{
	linkToTemplate(nullptr);
}


var CustomValuesCommandArgument::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("param", param->getJSONData());
	return data;
}

void CustomValuesCommandArgument::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	param->loadJSONData(data.getProperty("param", var()));
}

void CustomValuesCommandArgument::linkToTemplate(CustomValuesCommandArgument * t)
{
	if (linkedTemplate != nullptr && !linkedTemplateRef.wasObjectDeleted())
	{
		linkedTemplate->param->removeParameterListener(this);
		linkedTemplate->editable->removeParameterListener(this);
		linkedTemplate->useForMapping->removeParameterListener(this);
		linkedTemplate = nullptr;
	}

	linkedTemplate = t;

	if (linkedTemplate != nullptr)
	{
		linkedTemplate->param->addParameterListener(this);
		linkedTemplate->editable->addParameterListener(this);
		linkedTemplate->useForMapping->addParameterListener(this);
	}

	if (param != nullptr) param->saveValueOnly = linkedTemplate != nullptr;
	
	canBeReorderedInEditor = linkedTemplate == nullptr;
}

void CustomValuesCommandArgument::updateParameterFromTemplate()
{
	if (linkedTemplate != nullptr)
	{
		param->setControllableFeedbackOnly(!linkedTemplate->editable->boolValue());

		param->setRange(linkedTemplate->param->minimumValue, linkedTemplate->param->maximumValue);

		if (useForMapping != nullptr && linkedTemplate->useForMapping != nullptr) useForMapping->setValue(linkedTemplate->useForMapping);
		
		if (param->isControllableFeedbackOnly || !param->isOverriden)
		{
			if (param->type == Controllable::ENUM)
			{
				EnumParameter * lep = (EnumParameter *)linkedTemplate->param;
				EnumParameter * ep = (EnumParameter *)param;
				ep->clearOptions();
				for (auto &ev : lep->enumValues) ep->addOption(ev->key, ev->value);
			}

			param->defaultValue = linkedTemplate->param->value;
			param->resetValue();
		}
	}
}

void CustomValuesCommandArgument::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == useForMapping)
	{
		argumentListeners.call(&ArgumentListener::useForMappingChanged, this);
	}
}

void CustomValuesCommandArgument::onExternalParameterValueChanged(Parameter * p)
{
	if (p->parentContainer == linkedTemplate) updateParameterFromTemplate();
}

String CustomValuesCommandArgument::getTypeString() const
{
	return param->getTypeString();
}



InspectableEditor * CustomValuesCommandArgument::getEditor(bool isRoot)
{
	return new CustomValuesCommandArgumentEditor(this, isRoot);
}
