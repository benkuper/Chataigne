/*
  ==============================================================================

	BaseCommand.cpp
	Created: 3 Nov 2016 10:50:32am
	Author:  bkupe

  ==============================================================================
*/

#include "BaseCommand.h"
#include "Template/CommandTemplateManager.h"
#include "Module/Module.h"
#include "ui/BaseCommandEditor.h"

BaseCommand::BaseCommand(Module* _module, CommandContext _context, var _params, Multiplex* multiplex) :
	ControllableContainer("Command"),
	MultiplexTarget(multiplex),
	context(_context),
	module(_module),
	moduleRef(_module),
	params(_params),
	saveAndLoadTargetMappings(false),
	linkedTemplate(nullptr),
	customValuesManager(nullptr)
{
	paramsCanBeLinked = isMultiplexed() || context == MAPPING;
	hideEditorHeader = true;
}

BaseCommand::~BaseCommand()
{
	paramLinkMap.clear();
	paramLinkMap.clear();
	paramLinks.clear();
	linkToTemplate(nullptr);
}


void BaseCommand::onControllableAdded(Controllable* c)
{
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		ParameterLink* pLink = new ParameterLink(p, multiplex);
		pLink->inputValueNames = inputNames;

		paramLinks.add(pLink);
		paramLinkMap.set(p, pLink);
		linkParamMap.set(pLink, p);

		commandListeners.call(&CommandListener::commandContentChanged);
	}
}

void BaseCommand::onControllableRemoved(Controllable* c)
{
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		if (paramLinkMap.contains(p))
		{
			ParameterLink* pLink = paramLinkMap[p];
			linkParamMap.remove(pLink);
			paramLinkMap.remove(p);
			paramLinks.removeObject(pLink);

			commandListeners.call(&CommandListener::commandContentChanged);
		}
	}
}

bool BaseCommand::isControllableMappable(Controllable* c)
{
	return c->type != c->TRIGGER || c->type != c->ENUM || c->type != c->CUSTOM;
}

void BaseCommand::linkToTemplate(CommandTemplate* ct)
{
	if (linkedTemplate != nullptr && !templateRef.wasObjectDeleted() && !linkedTemplate->isClearing)
	{
		linkedTemplate->removeInspectableListener(this);
		linkedTemplate->removeCommandTemplateListener(this);
		linkedTemplate = nullptr;
		templateRef = nullptr;
	}

	linkedTemplate = ct;
	templateRef = ct;

	if (linkedTemplate != nullptr)
	{
		linkedTemplate->addInspectableListener(this);
		linkedTemplate->addCommandTemplateListener(this);
		updateParametersFromTemplate();
	}
}

void BaseCommand::updateParametersFromTemplate()
{
	if (linkedTemplate == nullptr) return;

	for (auto& pc : linkedTemplate->paramsContainer.controllableContainers)
	{
		CommandTemplateParameter* ctp = dynamic_cast<CommandTemplateParameter*>(pc.get());
		updateParameterFromTemplate(ctp);
	}

	if (customValuesManager != nullptr)
	{
		customValuesManager->linkToTemplate(linkedTemplate->customValuesManager.get());
	}
}

void BaseCommand::updateParameterFromTemplate(CommandTemplateParameter* ctp)
{
	if (ctp == nullptr) return;

	String addr = ctp->getControlAddress(&linkedTemplate->paramsContainer);
	Parameter* p = dynamic_cast<Parameter*>(getControllableForAddress(addr));
	jassert(p != nullptr);
	p->setControllableFeedbackOnly(!ctp->editable->boolValue());

	p->defaultValue = ctp->parameter->value;
	if (p->isControllableFeedbackOnly || !p->isOverriden)
	{
		p->resetValue();
	}
}

void BaseCommand::setupTemplateParameters(CommandTemplate* ct)
{
	ct->paramsContainer.clear();

	Array<WeakReference<Parameter>> allParams = getAllParameters();
	for (auto& p : allParams)
	{
		if (!p->enabled || p->isControllableFeedbackOnly || p->hideInEditor) continue;
		CommandTemplateParameter* ctp = new CommandTemplateParameter(p);
		ct->paramsContainer.addChildControllableContainer(ctp);
		ct->templateParams.add(ctp);
	}


	for (auto& cc : controllableContainers)
	{
		if (cc == customValuesManager.get()) continue;
		ControllableContainer* cct = new ControllableContainer(cc->niceName);
		ct->paramsContainer.addChildControllableContainer(cct, true);

		Array<WeakReference<Parameter>> allParams = cc->getAllParameters();
		for (auto& p : allParams)
		{
			if (!p->enabled || p->isControllableFeedbackOnly || p->hideInEditor) continue;
			CommandTemplateParameter* ctp = new CommandTemplateParameter(p);
			cct->addChildControllableContainer(ctp);
			ct->templateParams.add(ctp);
		}
	}

	if (customValuesManager != nullptr)
	{
		//create customValuesTemplateManager
		ct->customValuesManager.reset(new CustomValuesCommandArgumentManager(context == MAPPING, true, multiplex));
		ct->customValuesManager->allowedTypes.addArray(customValuesManager->allowedTypes);
		ct->addChildControllableContainer(ct->customValuesManager.get());
	}
}

void BaseCommand::setUseCustomValues(bool value)
{
	if (value)
	{
		if (customValuesManager == nullptr)
		{
			customValuesManager.reset(new CustomValuesCommandArgumentManager(context == MAPPING, false, multiplex));
			customValuesManager->addBaseManagerListener(this);
			addChildControllableContainer(customValuesManager.get());
		}

	}
	else
	{
		if (customValuesManager != nullptr)
		{
			removeChildControllableContainer(customValuesManager.get());
			customValuesManager->removeBaseManagerListener(this);
			customValuesManager.reset();
		}
	}
}

void BaseCommand::templateParameterChanged(CommandTemplateParameter* ctp)
{
	updateParameterFromTemplate(ctp);
}

void BaseCommand::setMappingValueType(Controllable::Type type)
{
	if (valueType == type) return;
	valueType = type;
	commandListeners.call(&CommandListener::valueTypeChanged);
}

void BaseCommand::trigger(int multiplexIndex)
{
	if (moduleRef.wasObjectDeleted())
	{
		DBG("Module removed, not processing that");
		return;
	}

	triggerInternal(multiplexIndex);
}

void BaseCommand::setValue(var value, int multiplexIndex)
{
	for (auto& pLink : paramLinks) pLink->updateMappingInputValue(value, multiplexIndex);
	if (customValuesManager != nullptr)
	{
		for (auto& cv : customValuesManager->items)
		{
			if (cv->paramLink != nullptr) cv->paramLink->updateMappingInputValue(value, multiplexIndex);
		}
	}
	setValueInternal(value, multiplexIndex);
	trigger(multiplexIndex);
}

ParameterLink* BaseCommand::getLinkedParam(Parameter* p)
{
	jassert(paramLinkMap.contains(p));
	return paramLinkMap[p];
}

var BaseCommand::getLinkedValue(Parameter* p, int multiplexIndex)
{
	return getLinkedParam(p)->getLinkedValue(multiplexIndex);
}

void BaseCommand::linkParamToMappingIndex(Parameter* p, int mappingIndex)
{
	if (context != MAPPING) return;

	if (ParameterLink* pLink = getLinkedParam(p))
	{
		pLink->setLinkType(pLink->MAPPING_INPUT);
		pLink->mappingValueIndex = mappingIndex;
	}
}

void BaseCommand::setInputNamesFromParams(Array<WeakReference<Parameter>> outParams)
{
	inputNames.clear();
	for (int i = 0; i < outParams.size(); i++)
	{
		String tString = outParams[i]->getTypeString();
		if (outParams[i]->isComplex())
		{
			StringArray valueNames = outParams[i]->getValuesNames();
			for (auto& vName : valueNames) inputNames.add(tString + " (" + vName + ")");
		}
		else
		{
			inputNames.add(tString);
		}
	}
	
	for (auto& pLink : paramLinks) pLink->inputValueNames = inputNames;
	if (customValuesManager != nullptr) customValuesManager->setInputNames(inputNames);
}

var BaseCommand::getLinkedCustomArgumentValueAt(int argIndex, int multiplexIndex)
{
	if (customValuesManager == nullptr) return var();
	if (argIndex < 0 || argIndex >= customValuesManager->items.size()) return var();
	return customValuesManager->items[argIndex]->getLinkedValue(multiplexIndex);
}

void BaseCommand::inspectableDestroyed(Inspectable* i)
{
	if (i == templateRef)
	{
		commandListeners.call(&CommandListener::commandTemplateDestroyed);// linkToTemplate(nullptr);
	}
}

var BaseCommand::getJSONData()
{
	var data = ControllableContainer::getJSONData();

	var pLinkData(new DynamicObject());
	for (auto& pLink : paramLinks)
	{
		if (pLink->linkType != pLink->NONE) pLinkData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());
	}

	data.getDynamicObject()->setProperty("paramLinks", pLinkData);

	return data;
}

void BaseCommand::loadJSONDataInternal(var data)
{
	var pLinksData = data.getProperty("paramLinks", var());
	for (auto& pLink : paramLinks) pLink->loadJSONData(pLinksData.getProperty(pLink->parameter->shortName, var()));
}

BaseCommand* BaseCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	Module* m = dynamic_cast<Module*>(module);
	jassert(m != nullptr);
	CommandTemplate* commandTemplate = m->templateManager->getItemWithName(params.getProperty("template", ""));
	if (commandTemplate == nullptr)
	{
		LOGERROR("Template not found : " << params.getProperty("template", "").toString());
		return nullptr;
	}

	return commandTemplate->createCommand(m, context, params, multiplex);
}

InspectableEditor* BaseCommand::getEditor(bool isRoot)
{
	return new BaseCommandContainerEditor(this, this, isRoot);
}
