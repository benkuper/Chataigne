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
	param->isSavable = false; // save manually
	addControllable(param);

	if(templateMode) param->isCustomizableByUser = true;
	
	param->forceSaveValue = true;
	param->saveValueOnly = !templateMode;
	param->forceSaveRange = true;
	
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
	if (data.getDynamicObject()->hasProperty("param"))
	{
		param->loadJSONData(data.getProperty("param", var()));
	}
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
		if (!templateMode) updateParameterFromTemplate();
	}
	

	if (param != nullptr)
	{
		param->saveValueOnly = linkedTemplate != nullptr;
		param->isCustomizableByUser = linkedTemplate == nullptr;
	}

	canBeReorderedInEditor = linkedTemplate == nullptr;
	userCanRemove = linkedTemplate == nullptr;
	userCanDuplicate = linkedTemplate == nullptr;
	nameCanBeChangedByUser = linkedTemplate == nullptr;


}

void CustomValuesCommandArgument::updateParameterFromTemplate()
{
	if (linkedTemplate != nullptr)
	{
		param->setControllableFeedbackOnly(!linkedTemplate->editable->boolValue());
		if(linkedTemplate->param->hasRange()) param->setRange(linkedTemplate->param->minimumValue, linkedTemplate->param->maximumValue);

		if (useForMapping != nullptr && linkedTemplate->useForMapping != nullptr && !useForMapping->isOverriden) useForMapping->setValue(linkedTemplate->useForMapping->boolValue());
		
		param->defaultValue = linkedTemplate->param->value;


		if (param->isControllableFeedbackOnly || !param->isOverriden)
		{
			if (param->type == Controllable::ENUM)
			{
				EnumParameter * lep = (EnumParameter *)linkedTemplate->param;
				EnumParameter * ep = (EnumParameter *)param;
				ep->clearOptions();
				for (auto &ev : lep->enumValues) ep->addOption(ev->key, ev->value);
			}
			
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

void CustomValuesCommandArgument::onExternalParameterRangeChanged(Parameter* p)
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
